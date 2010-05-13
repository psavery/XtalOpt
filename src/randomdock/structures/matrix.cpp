/**********************************************************************
  RandomDock - Matrix: Wrapper for Avogadro::Molecule to hold the 
  matrix monomers in a docking problem

  Copyright (C) 2009 by David C. Lonie

  This file is part of the Avogadro molecular editor project.
  For more information, see <http://avogadro.openmolecules.net/>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 ***********************************************************************/

#include "matrixmol.h"

#include <avogadro/molecule.h>

#include <openbabel/rand.h>
#include <openbabel/mol.h>

#include <QDebug>
#include <QProgressDialog>

using namespace std;

namespace Avogadro {

  Matrix::Matrix(QObject *parent) : 
    Molecule(parent), m_probs(0)
  {
    qDebug() << "Matrix::Matrix( " << parent << " ) called";
    m_probs = new QList<double>;
  }

  Matrix::Matrix(Molecule *mol) {
    qDebug() << "Matrix::Matrix( [copy " << &mol << "] ) called";

    m_probs = new QList<double>;

    OpenBabel::OBMol obmol = mol->OBMol();

    setOBMol(&obmol);

    generateProbabilities();    
  }

  Matrix::~Matrix() {
    delete m_probs;
  }

  void Matrix::load(QTextStream &in) {
    Q_UNUSED(in);
    qDebug() << "Matrix::load( ) called";
    generateProbabilities();
  }

  void Matrix::save(QTextStream &in) {
    Q_UNUSED(in);
    qDebug() << "Matrix::save( ) called";
    // Nothing to do here, yet...
  }

  void Matrix::sortConformers() {
    qDebug() << "Matrix::sortConformers( ) called";
    std::vector<Eigen::Vector3d> tmp;
    double tmp_e;

    // Make sure that energies().size() == numConformers:
    energies();

    QProgressDialog prog ("Sorting conformers by energy...", 0, 0, numConformers(), NULL);

    // Simple selection sort
    for (uint i = 0; i < numConformers(); i++) {
      prog.setValue(i);
      for (uint j = i; j < numConformers(); j++) {
        if (energy(j) < energy(i)) {
          tmp = *conformer(i);
          addConformer(*conformer(j), i);
          addConformer(tmp, j);
          tmp_e = energy(i);
          setEnergy(i, energy(j));
          setEnergy(j, tmp_e);
        }
      }
    }
  }

  void Matrix::generateProbabilities() {
    qDebug() << "Matrix::generateProbabilities( ) called";

    sortConformers();
    m_probs->clear();
    double lowest = energy(0);;
    double highest = energy(numConformers() - 1);
    double spread = highest - lowest + (highest - lowest) / static_cast<double>(numConformers());
    // Generate a list of floats from 0->1 proportional to the energies;
    // E.g. if energies are:
    // -5   -2   -1   3   5
    // We'll have:
    // 0   0.3  0.4  0.8  1
    for (uint i = 0; i < numConformers(); i++)
        m_probs->append( ( energy(i) - lowest ) / spread);
    // Subtract each value from one, and find the sum of the resulting list
    // We'll end up with:
    // 1  0.7  0.6  0.2  0  --   sum = 2.5
    double sum = 0;
    for (int i = 0; i < m_probs->size(); i++){
      m_probs->replace(i, 1.0 - m_probs->at(i));
      sum += m_probs->at(i);
    }
    // Normalize with the sum so that the list adds to 1
    // 0.4  0.28  0.24  0.08  0
    for (int i = 0; i < m_probs->size(); i++){
      m_probs->replace(i, m_probs->at(i) / sum);
    }
    // Then replace each entry with a cumulative total:
    // 0.4 0.68 0.92 1 1
    sum = 0;
    for (int i = 0; i < m_probs->size(); i++){
      sum += m_probs->at(i);
      m_probs->replace(i, sum);
    }
    // And we have a enthalpy weighted probability list! To use:
    //
    //   double r = rand.NextFloat();
    //   uint ind;
    //   for (ind = 0; ind < m_probs->size(); ind++)
    //     if (r < m_probs->at(ind)) break;
    //
    // ind will hold the chosen index.
    //
    // Alternatively, the percent probability can be recovered as such:
    // 
    //   QList<double> percents;
    //   for (int i = 0; i < m_probs->size(); i++) {
    //     if (i == 0) percents.append(m_probs->at(i) * 100.0);
    //     else percents.append( ( m_probs->at(i) - m_probs->at(i-1) ) * 100.0 )
    //   }
    //
    // percents will hold the percent probabilities
  }
    
  Matrix* Matrix::getRandomConformer() {
    qDebug() << "Matrix::getRandomConformer( ) called";
    // Random number generator
    OpenBabel::OBRandom rand (true); 	// "true" uses system random numbers. OB's version isn't too good...
    rand.TimeSeed();

    // Select conformer to use:
    double r = rand.NextFloat();
    int ind;
    for (ind = 0; ind < m_probs->size(); ind++)
      if (r < m_probs->at(ind)) break;

    // Generate new matrix from current:
    Matrix *sub = new Matrix (this);
    sub->setConformer(r);
    return sub;
  }

} // end namespace Avogadro

#include "matrixmol.moc"