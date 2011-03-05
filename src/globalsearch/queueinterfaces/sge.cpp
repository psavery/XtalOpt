/**********************************************************************
  SgeQueueInterface - Base class for running jobs remotely on a SGE
  cluster.

  Copyright (C) 2011 by David C. Lonie

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 ***********************************************************************/

#include <globalsearch/queueinterfaces/sge.h>

#include <globalsearch/optimizer.h>
#include <globalsearch/macros.h>
#include <globalsearch/sshconnection.h>
#include <globalsearch/sshmanager.h>
#include <globalsearch/structure.h>

namespace GlobalSearch {

  SgeQueueInterface::SgeQueueInterface(OptBase *parent,
                                       const QString &settingsFile) :
    RemoteQueueInterface(parent, settingsFile),
    m_qstat("qstat"),
    m_qsub("qsub"),
    m_qdel("qdel")
  {
    m_idString = "SGE";
    m_templates.append("job.sh");
    m_hasDialog = true;

    readSettings(settingsFile);
  }

  SgeQueueInterface::~SgeQueueInterface()
  {
  }

  QDialog* SgeQueueInterface::dialog()
  {
    if (!m_dialog) {
      m_dialog = new SgeConfigDialog (m_opt->dialog(),
                                      m_opt,
                                      this);
    }
    SgeConfigDialog *d = qobject_cast<SgeConfigDialog*>(m_dialog);
    d->updateGUI();

    return d;
  }

  void SgeQueueInterface::readSettings(const QString &filename)
  {
    SETTINGS(filename);

    settings->beginGroup(m_opt->getIDString().toLower());
    settings->beginGroup("queueinterface/sgequeueinterface");
    int loadedVersion = settings->value("version", 0).toInt();
    settings->beginGroup("paths");

    m_qsub  = settings->value("qsub",  "qsub").toString();
    m_qstat = settings->value("qstat", "qstat").toString();
    m_qdel  = settings->value("qdel",  "qdel").toString();

    settings->endGroup();
    settings->endGroup();
    settings->endGroup();

    DESTROY_SETTINGS(filename);

    // Update config data
    switch (loadedVersion) {
    case 0: // Load old stuff from /sys/ block
      settings->beginGroup(m_opt->getIDString().toLower());
      settings->beginGroup("sys");
      m_qsub = settings->value("queue/qsub", "qsub").toString();
      m_qstat = settings->value("queue/qstat", "qstat").toString();
      m_qdel = settings->value("queue/qdel", "qdel").toString();
      settings->endGroup();
      settings->endGroup();
    case 1:
    default:
      break;
    }

  }

  void SgeQueueInterface::writeSettings(const QString &filename)
  {
    SETTINGS(filename);

    const int VERSION = 1;

    settings->beginGroup(m_opt->getIDString().toLower());
    settings->beginGroup("queueinterface/sgequeueinterface");
    settings->setValue("version", VERSION);
    settings->beginGroup("paths");

    settings->setValue("qsub",  m_qsub);
    settings->setValue("qstat", m_qstat);
    settings->setValue("qdel",  m_qdel);

    settings->endGroup();
    settings->endGroup();
    settings->endGroup();

    DESTROY_SETTINGS(filename);
  }

  bool SgeQueueInterface::startJob(Structure *s)
  {
    SSHConnection *ssh = m_opt->ssh()->getFreeConnection();

    if (!ssh->reconnectIfNeeded()) {
      m_opt->warning(tr("Cannot connect to ssh server %1@%2:%3")
                     .arg(ssh->getUser())
                     .arg(ssh->getHost())
                     .arg(ssh->getPort())
                     );
      m_opt->ssh()->unlockConnection(ssh);
      return false;
    }

    QWriteLocker wlocker (s->lock());

    QString command = "cd \"" + s->getRempath() + "\" && " +
      m_qsub + " job.sh";

    QString stdout_str;
    QString stderr_str;
    int ec;
    if (!ssh->execute(command, stdout_str, stderr_str, ec) || ec != 0) {
      m_opt->warning(tr("Error executing %1: %2")
                     .arg(command).arg(stderr_str));
      m_opt->ssh()->unlockConnection(ssh);
      return false;
    }
    m_opt->ssh()->unlockConnection(ssh);

    // Assuming stdout_str value is
    // your job <jobID> ('job.sh') has been submitted
    unsigned int jobID = stdout_str.split(QRegExp("\\s+"))[2].toUInt();

    s->setJobID(jobID);
    s->startOptTimer();
    return true;
  }

  bool SgeQueueInterface::stopJob(Structure *s)
  {
    SSHConnection *ssh = m_opt->ssh()->getFreeConnection();

    if (!ssh->reconnectIfNeeded()) {
      m_opt->warning(tr("Cannot connect to ssh server %1@%2:%3")
                     .arg(ssh->getUser())
                     .arg(ssh->getHost())
                     .arg(ssh->getPort())
                     );
      m_opt->ssh()->unlockConnection(ssh);
      return false;
    }

    // lock structure
    QWriteLocker locker (s->lock());

    // jobid has not been set, cannot delete!
    if (s->getJobID() == 0) {
      m_opt->ssh()->unlockConnection(ssh);
      return true;
    }

    // TODO Allow a path to be added here if needed
    const QString command = m_qdel + " " + QString::number(s->getJobID());

    // Execute
    QString stdout_str;
    QString stderr_str;
    int ec;
    if (!ssh->execute(command, stdout_str, stderr_str, ec) || ec != 0) {
      // Most likely job is already gone from queue. Set jobID to 0.
      m_opt->warning(tr("Error executing %1 (this can likely be ignored): %2")
                     .arg(command).arg(stderr_str));
      s->setJobID(0);
      m_opt->ssh()->unlockConnection(ssh);
      return false;
    }

    s->setJobID(0);
    s->stopOptTimer();
    m_opt->ssh()->unlockConnection(ssh);
    return true;
  }

  QueueInterface::QueueStatus SgeQueueInterface::getStatus(Structure *s) const
  {
    // lock structure
    QWriteLocker locker (s->lock());
    QStringList queueData = getQueueList();
    unsigned int jobID = static_cast<unsigned int>(s->getJobID());

    // If the queueData cannot be fetched, queueData contains a single
    // string, "CommError"
    if (queueData.size() == 1 && queueData[0].compare("CommError") == 0) {
      return QueueInterface::CommunicationError;
    }

    // If jobID = 0 and structure is not in "Submitted" state, return an error.
    if (!jobID && s->getStatus() != Structure::Submitted) {
      return QueueInterface::Error;
    }

    // Determine status if structure is in the queue. queueData
    // entries are of the following format:
    //
    // job-ID prior name  user  state submit/start at queue   function
    // 231    0     hydra craig r     07/13/96        durin.q MASTER
    //                                20:27:15
    //
    // (Note that the whitespace has been condensed in the above)
    QString status;
    QStringList list;
    for (int i = 0; i < queueData.size(); i++) {
      list = queueData.at(i).split(QRegExp("\\s+"));
      if (list[0].toUInt() == jobID) {
        status = list[4];
        continue;
      }
    }

    // If structure is submitted, check if it is in the queue. If not,
    // check if the completion file has been written.
    //
    // If the completion file exists, then the job finished before the
    // queue checks could see it, and the function will continue on to
    // the status checks below.
    //
    // If the structure in Submitted state
    if (s->getStatus() == Structure::Submitted) {
      // and the jobID isn't in the queue
      if (status.isEmpty()) {
        // check if the output file is absent
        bool exists;
        if (!m_opt->optimizer()->checkIfOutputFileExists(s, &exists)) {
          return QueueInterface::CommunicationError;
        }
        if (!exists) {
          // The job is still pending
          return QueueInterface::Pending;
        }
        else {
          // The job has completed.
          return QueueInterface::Started;
        }
      }
      else {
        // The job is in the queue
        return QueueInterface::Started;
      }
    }

    if (status.contains('r')) {
      return QueueInterface::Running;
    }
      else if (status.contains(QRegExp("q|w|s"))) {
      return QueueInterface::Queued;
    }
    else { // Entry is missing from queue. Were the output files written?
      locker.unlock();
      bool outputFileExists;
      if (!m_opt->optimizer()->checkIfOutputFileExists(s, &outputFileExists) ) {
          return QueueInterface::CommunicationError;
      }
      locker.relock();

      if (outputFileExists) {
        // Did the job finish successfully?
        bool success;
        if (!m_opt->optimizer()->checkForSuccessfulOutput(s, &success)) {
          return QueueInterface::CommunicationError;
        }
        if (success) {
          return QueueInterface::Success;
        }
        else {
          return QueueInterface::Error;
        }
      }
    }
    // Not in queue and no output?
    return QueueInterface::Unknown;
  }

  QStringList SgeQueueInterface::getQueueList() const
  {
    // recast queue mutex as mutable for safe access:
    QReadWriteLock &queueMutex = const_cast<QReadWriteLock&> (m_queueMutex);

    queueMutex.lockForRead();
    // Limit queries to once per second
    if (m_queueTimeStamp.isValid() &&
        m_queueTimeStamp.msecsTo(QDateTime::currentDateTime())
        <= 1000) {
      // If the cache is valid, return it
      QStringList ret (m_queueData);
      queueMutex.unlock();
      return ret;
    }

    // Otherwise, store a copy of the current timestamp and switch
    // queuemutex to writelock
    QDateTime oldTimeStamp (m_queueTimeStamp);
    queueMutex.unlock();

    // Relock mutex
    QWriteLocker queueLocker (&queueMutex);

    // Non-fatal assert: Check current timestamp against the
    // oldTimeStamp from earlier. If they don't match, another thread
    // has already updated the queueData, so tail-recurse this
    // function and try again.
    if (m_queueTimeStamp.msecsTo(oldTimeStamp) != 0) {
      queueLocker.unlock();
      return this->getQueueList();
    }

    // Queue will be updated -- cast queue cache and timestamp as
    // mutable
    QStringList &queueData     = const_cast<QStringList&> (m_queueData);
    QDateTime &queueTimeStamp  = const_cast<QDateTime&> (m_queueTimeStamp);

    // Get SSH connection
    SSHConnection *ssh = m_opt->ssh()->getFreeConnection();

    if (!ssh->reconnectIfNeeded()) {
      m_opt->warning(tr("Cannot connect to ssh server %1@%2:%3")
                     .arg(ssh->getUser())
                     .arg(ssh->getHost())
                     .arg(ssh->getPort())
                     );
      m_opt->ssh()->unlockConnection(ssh);
      queueTimeStamp = QDateTime::currentDateTime();
      queueData.clear();
      queueData << "CommError";
      QStringList ret (m_queueData);
      return ret;
    }

    QString command = m_qstat + " | grep " + m_opt->username;

    // Execute
    QString stdout_str;
    QString stderr_str;
    int ec;
    // Valid exit codes for grep: (0) matches found, execution successful
    //                            (1) no matches found, execution successful
    //                            (2) execution unsuccessful
    if (!ssh->execute(command, stdout_str, stderr_str, ec)
        || (ec != 0 && ec != 1 )
        ) {
      m_opt->ssh()->unlockConnection(ssh);
      m_opt->warning(tr("Error executing %1: (%2) %3\n\t"
                        "Using cached queue data.")
                     .arg(command)
                     .arg(QString::number(ec))
                     .arg(stderr_str));
      queueTimeStamp = QDateTime::currentDateTime();
      QStringList ret (m_queueData);
      return ret;
    }
    m_opt->ssh()->unlockConnection(ssh);

    queueData = stdout_str.split("\n", QString::SkipEmptyParts);

    QStringList ret (m_queueData);
    queueTimeStamp = QDateTime::currentDateTime();
    return ret;
  }

}
