/**********************************************************************
  GAPC -- A genetic algorithm for protected clusters

  Copyright (C) 2009-2010 by David Lonie

  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Library General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 ***********************************************************************/

#include <gapc/ui/dialog.h>

#include <gapc/gapc.h>

#include <globalsearch/tracker.h>

#include <gapc/ui/tab_init.h>
#include <gapc/ui/tab_edit.h>
#include <gapc/ui/tab_opt.h>
#include <gapc/ui/tab_progress.h>
#include <gapc/ui/tab_plot.h>
#include <gapc/ui/tab_log.h>

#include <avogadro/glwidget.h>

#include <openbabel/oberror.h>

#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QtConcurrentRun>
#include <QSemaphore>

using namespace GlobalSearch;
using namespace Avogadro;
using namespace std;

namespace GAPC {

  GAPCDialog::GAPCDialog( GLWidget *glWidget,
                                QWidget *parent,
                                Qt::WindowFlags f ) :
    AbstractDialog( glWidget, parent, f )
  {
    ui.setupUi(this);
    ui_push_begin   = ui.push_begin;
    ui_push_save    = ui.push_save;
    ui_push_resume  = ui.push_resume;
    ui_label_opt    = ui.label_opt;
    ui_label_run    = ui.label_run;
    ui_label_fail   = ui.label_fail;
    ui_label_prog   = ui.label_prog;
    ui_progbar      = ui.progbar;
    ui_tabs         = ui.tabs;

    // Initialize vars, connections, etc
    OptGAPC *gapc = new OptGAPC (this);

    m_opt = gapc;

    // Initialize tabs
    m_tab_init		= new TabInit(this, gapc);
    m_tab_edit		= new TabEdit(this, gapc);
    m_tab_opt		= new TabOpt(this, gapc);
    m_tab_progress	= new TabProgress(this, gapc);
    m_tab_plot		= new TabPlot(this, gapc);
    m_tab_log		= new TabLog(this, gapc);

    // Populate tab widget
    ui.tabs->clear();
    ui.tabs->addTab(m_tab_init->getTabWidget(),		tr("Cell &Initialization"));
    ui.tabs->addTab(m_tab_edit->getTabWidget(),         tr("Optimization &Templates"));
    ui.tabs->addTab(m_tab_opt->getTabWidget(),          tr("&Optimization Settings"));
    ui.tabs->addTab(m_tab_progress->getTabWidget(),     tr("&Progress"));
    ui.tabs->addTab(m_tab_plot->getTabWidget(),         tr("&Plot"));
    ui.tabs->addTab(m_tab_log->getTabWidget(),          tr("&Log"));

    initialize();
  }

  GAPCDialog::~GAPCDialog()
  {
    this->hide();

    m_opt->tracker()->lockForRead();
    writeSettings();
    saveSession();
    m_opt->tracker()->unlock();
    // m_opt is deleted by ~AbstractDialog
  }

  void GAPCDialog::setMolecule(Molecule *molecule)
  {
    if (m_molecule == molecule || !molecule) {
      return;
    }
    m_molecule = molecule;
  }

  void GAPCDialog::saveSession() {
    // Notify if this was user requested.
    bool notify = false;
    if (sender() == ui.push_save) {
      notify = true;
    }
    if (m_opt->savePending) {
      return;
    }
    m_opt->savePending = true;
    QtConcurrent::run(m_opt,
                      &OptBase::save,
                      QString(""),
                      notify);
  }

  void GAPCDialog::startSearch() {
    QtConcurrent::run(qobject_cast<OptGAPC*>(m_opt),
                      &OptGAPC::startSearch);
  }

}
