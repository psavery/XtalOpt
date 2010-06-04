/**********************************************************************
  RandomDock -- A tool for analysing a matrix-substrate docking problem

  Copyright (C) 2009 by David Lonie

  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Library General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 ***********************************************************************/

#include "tab_conformers.h"

#include "dialog.h"
#include "../randomdock.h"
#include "../structures/matrix.h"
#include "../structures/substrate.h"
#include "../../generic/macros.h"
#include "../../generic/structure.h"

#include <openbabel/forcefield.h>

#include <avogadro/atom.h>
#include <avogadro/molecule.h>

#include <QSettings>
#include <QMessageBox>
#include <QtConcurrentRun>

using namespace std;
using namespace Avogadro;

namespace RandomDock {

  TabConformers::TabConformers( RandomDockDialog *dialog, RandomDock *opt) :
    QObject(dialog),
    m_dialog(dialog),
    m_opt(opt),
    m_ff(0)
  {
    m_tab_widget = new QWidget;
    ui.setupUi(m_tab_widget);

    m_ff = OpenBabel::OBForceField::FindForceField("MMFF94");

    // dialog connections
    connect(m_dialog, SIGNAL(tabsReadSettings(const QString &)),
            this, SLOT(readSettings(const QString &)));
    connect(m_dialog, SIGNAL(tabsWriteSettings(const QString &)),
            this, SLOT(writeSettings(const QString &)));
    connect(m_dialog, SIGNAL(tabsUpdateGUI()),
            this, SLOT(updateGUI()));
    connect(m_dialog, SIGNAL(tabsDisconnectGUI()),
            this, SLOT(disconnectGUI()));
    connect(m_dialog, SIGNAL(tabsLockGUI()),
            this, SLOT(lockGUI()));
    connect(this, SIGNAL(moleculeChanged(Structure*)),
            m_dialog, SIGNAL(moleculeChanged(Structure*)));

    // tab connections
    connect(ui.push_generate, SIGNAL(clicked()),
            this, SLOT(generateConformers()));
    connect(ui.push_refresh, SIGNAL(clicked()),
            this, SLOT(updateStructureList()));
    connect(ui.combo_mol, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(selectStructure(QString)));
    connect(ui.table_conformers, SIGNAL(currentCellChanged(int, int, int, int)),
            this, SLOT(conformerChanged(int, int, int, int)));
    connect(ui.cb_allConformers, SIGNAL(toggled(bool)),
            this, SLOT(calculateNumberOfConformers(bool)));
    connect(this, SIGNAL(conformersChanged()),
            this, SLOT(updateConformerTable()));
  }

  TabConformers::~TabConformers()
  {
    qDebug() << "TabConformers::~TabConformers() called";
    writeSettings();
  }

  void TabConformers::writeSettings(const QString &filename)
  {
    SETTINGS(filename);
    settings->beginGroup("randomdock/conformers");

    settings->setValue("number",      	ui.spin_nConformers->value());
    settings->setValue("all",      	ui.cb_allConformers->isChecked());

    settings->endGroup();
    DESTROY_SETTINGS(filename);
  }

  void TabConformers::readSettings(const QString &filename)
  {
    SETTINGS(filename);
    settings->beginGroup("randomdock/conformers");

    ui.spin_nConformers->setValue(	settings->value("number",	100).toInt());
    ui.cb_allConformers->setChecked(	settings->value("all",		true).toInt());

    settings->endGroup();      
  }

  void TabConformers::updateGUI()
  {
    updateStructureList();
  }

  void TabConformers::disconnectGUI()
  {
  }

  void TabConformers::lockGUI()
  {
    ui.push_refresh->setDisabled(true);
    ui.push_generate->setDisabled(true);
    ui.combo_opt->setDisabled(true);
    ui.spin_nConformers->setDisabled(true);
    ui.cb_allConformers->setDisabled(true);
  }

  void TabConformers::updateStructureList() {
    qDebug() << "TabConformers::updateStructureList() called";
    ui.combo_mol->clear();
    qDebug() << "substrate: " << m_opt->substrate;
    if (m_opt->substrate)
      ui.combo_mol->addItem("Substrate");
    for (int i = 0; i < m_opt->matrixList.size(); i++) {
      ui.combo_mol->addItem(tr("Matrix %1").arg(i+1));
    }
  }

  void TabConformers::generateConformers()
  {
    // Enable this when conformer generation is complete
    ui.push_generate->setEnabled(false);
    QtConcurrent::run(this,
                      &TabConformers::generateConformers_,
                      currentStructure());
  }

  void TabConformers::generateConformers_(Structure *mol)
  {
    m_dialog->startProgressUpdate("Preparing conformer search...", 0, 0);
    if (ui.combo_opt->currentIndex() == O_G03) {
      // TODO: implement and use a G03 opt routine...
      ui.push_generate->setEnabled(true);
      return;
    }
    
    /******************************************************
     * Most of this is shamelessly taken from Avogadro's  *
     * forcefield extension...                            *
     ******************************************************/

    m_dialog->updateProgressLabel("Preparing forcefield...");
    OpenBabel::OBForceField *ff = m_ff->MakeNewInstance();
    ff->SetLogFile(&std::cout);
    ff->SetLogLevel(OBFF_LOGLVL_NONE);

    m_dialog->updateProgressLabel("Setting up molecule with forcefield...");
    OpenBabel::OBMol obmol = mol->OBMol();
    if (!ff) {
      QMessageBox::warning( m_dialog, tr( "Avogadro" ),
                            tr( "Problem setting up forcefield '%1'.")
                            .arg(ui.combo_opt->currentText().trimmed()));
      m_dialog->stopProgressUpdate();
      ui.push_generate->setEnabled(true);
      return;
    }
    if (!ff->Setup(obmol)) {
      QMessageBox::warning( m_dialog, tr( "Avogadro" ),
                            tr( "Cannot set up the force field for this molecule." ));
      m_dialog->stopProgressUpdate();
      ui.push_generate->setEnabled(true);
      return;
    }

    // Pre-optimize
    m_dialog->updateProgressLabel("Pre-optimizing molecule...");
    ff->ConjugateGradients(1000);

    // Prepare progress step variable
    int step = 0;

    // Systematic search:
    if (ui.cb_allConformers->isChecked()) {
      m_dialog->updateProgressLabel(tr("Performing systematic rotor search..."));
      int n = ff->SystematicRotorSearchInitialize(2500);
      m_dialog->updateProgressMaximum(2*n);
      while (ff->SystematicRotorSearchNextConformer(500)) {
        m_dialog->updateProgressValue(++step);
      }
    }
    // Random conformer search
    else {
      int n = ui.spin_nConformers->value();
      m_dialog->updateProgressLabel(tr("Performing random rotor search..."));
      ff->RandomRotorSearchInitialize(n, 2500);
      m_dialog->updateProgressMaximum(2*n);
      while (ff->RandomRotorSearchNextConformer(500)) {
        m_dialog->updateProgressValue(++step);
      }
    }
    obmol = mol->OBMol();
    // copy conformers ff-->obmol
    ff->GetConformers( obmol );
    // copy conformer obmol-->mol
    std::vector<Eigen::Vector3d> conformer;
    std::vector<std::vector<Eigen::Vector3d>*> confs;
    std::vector<double> energies;

    for (int i = 0; i < obmol.NumConformers(); i++) {
      m_dialog->updateProgressValue(++step);
      double *coordPtr = obmol.GetConformer(i);
      conformer.clear();
      foreach (Atom *atom, mol->atoms()) {
        while (conformer.size() < atom->id())
          conformer.push_back(Eigen::Vector3d(0.0, 0.0, 0.0));
        conformer.push_back(Eigen::Vector3d(coordPtr));
        coordPtr += 3;
      }
      mol->addConformer(conformer, i);
      mol->setEnergy(i, obmol.GetEnergy(i));
      confs.push_back(&conformer);
      energies.push_back(obmol.GetEnergy(i));
    }
    delete ff;
    emit conformersChanged();
    ui.push_generate->setEnabled(true);
    m_dialog->stopProgressUpdate();
  }

  void TabConformers::selectStructure(const QString & text) {
    qDebug() << "TabConformers::selectStructure( " << text << " ) called";
    Structure* mol = currentStructure();
    if (mol) {
      emit moleculeChanged(mol);
      updateConformerTable();
      calculateNumberOfConformers(ui.cb_allConformers->isChecked());
    }
  }

  void TabConformers::updateConformerTable() {
    qDebug() << "TabConformers::updateConformerTable() called";

    Structure *mol = currentStructure();

    // Generate probability lists:
    QList<double> tmp, probs;
    for (uint i = 0; i < mol->numConformers(); i++) {
      if (ui.combo_mol->currentText().contains("Substrate"))
        tmp.append(qobject_cast<Substrate*>(mol)->prob(i));
      else if (ui.combo_mol->currentText().contains("Matrix"))
        tmp.append(qobject_cast<Matrix*>(mol)->prob(i));
      else tmp.append(0);
    }
    for (int i = 0; i < tmp.size(); i++)
      if (i == 0) probs.append(tmp.at(i));
      else probs.append(tmp.at(i) - tmp.at(i-1));

    // Clear table
    while (ui.table_conformers->rowCount() != 0)
      ui.table_conformers->removeRow(0);

    // Fill table
    for (uint i = 0; i < mol->numConformers(); i++) {
      ui.table_conformers->insertRow(i);
      ui.table_conformers->setItem(i, Conformer, new QTableWidgetItem(QString::number(i+1)));
      ui.table_conformers->setItem(i, Energy, new QTableWidgetItem(QString::number(mol->energy(i))));
      ui.table_conformers->setItem(i, Prob, new QTableWidgetItem(QString::number(probs.at(i) * 100) + " %"));
    }
  }

  void TabConformers::conformerChanged(int row, int, int oldrow, int) {
    qDebug() << "TabConformers::conformerChanged( " << row << ", " << oldrow << " ) called";
    if (row == oldrow)	return;
    if (row == -1)	{emit moleculeChanged(new Structure); return;}
    Structure *mol = currentStructure();
    mol->setConformer(row);
    emit moleculeChanged(mol);
  }

  Structure* TabConformers::currentStructure() {
    qDebug() << "TabConformers::currentStructure() called";
    Structure *mol;
    QString text = ui.combo_mol->currentText();
    if (text == "Substrate")
      mol = qobject_cast<Structure*>(m_opt->substrate);
    else if (text.contains("Matrix")) {
      int ind = text.split(" ")[1].toInt() - 1;
      if (ind + 1 > m_opt->matrixList.size())
        mol = new Structure;
      else
        mol = qobject_cast<Structure*>(m_opt->matrixList.at(ind));
    }
    else mol = new Structure;
    return mol;
  }

  void TabConformers::calculateNumberOfConformers(bool isSystematic) {
    qDebug() << "TabConformers::calculateNumberOfConformers( " << isSystematic << " ) called";

    // If we don't want a systematic search, let the user pick the number of conformers
    if (!isSystematic) return;

    // I probably will never add the external conformer search....
    if (ui.combo_opt->currentIndex() == O_G03) return;

    // If there aren't any atoms in the structure, don't bother checking either.
    if (currentStructure()->numAtoms() == 0) return;

    OpenBabel::OBForceField *ff = m_ff->MakeNewInstance();
    ff->SetLogFile(&std::cout);
    ff->SetLogLevel(OBFF_LOGLVL_NONE);

    Structure* mol = currentStructure();
    OpenBabel::OBMol obmol = mol->OBMol();
    qDebug() << obmol.NumAtoms();
    if (!ff) {
      QMessageBox::warning( m_dialog, tr( "Avogadro" ),
                            tr( "Problem setting up forcefield '%1'.")
                            .arg(ui.combo_opt->currentText().trimmed()));
      return;
    }
    if (!ff->Setup(obmol)) {
      QMessageBox::warning( m_dialog, tr( "Avogadro" ),
                            tr( "Cannot set up the force field for this molecule." ));
      return;
    }
    int n = ff->SystematicRotorSearchInitialize();
    ui.spin_nConformers->setValue(n);
    delete ff;
  }
}

//#include "tab_conformers.moc"
