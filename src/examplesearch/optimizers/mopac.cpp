/**********************************************************************
  MopacOptimizer - Tools to interface with MOPAC

  Copyright (C) 2012 by David C. Lonie

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 ***********************************************************************/

#include <examplesearch/optimizers/mopac.h>

#include <globalsearch/structure.h>

#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

#include <QDebug>
#include <QDir>
#include <QString>

using namespace GlobalSearch;

namespace ExampleSearch {

MopacOptimizer::MopacOptimizer(OptBase* parent, const QString& filename)
  : Optimizer(parent)
{
  // Set allowed data structure keys, if any, e.g.
  // None here!

  // Set allowed filenames, e.g.
  m_templates.insert("job.mop", QStringList(""));

  // Setup for completion values
  m_completionFilename = "job.out";
  m_completionStrings.clear();
  m_completionStrings.append("          FINAL HEAT OF FORMATION =");

  // Set output filenames to try to read data from
  m_outputFilenames.append(m_completionFilename);
  m_outputFilenames.append("job.arc");

  // Set the name of the optimizer to be returned by getIDString()
  m_idString = "MOPAC";

  // Local execution setup:
  m_localRunCommand = "mopac job";
  m_stdinFilename = "";
  m_stdoutFilename = "";
  m_stderrFilename = "";

  readSettings(filename);
}

} // end namespace ExampleSearch
