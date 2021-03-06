#include "zdviddialog.h"

#include <iostream>
#include <QInputDialog>
#include "neutubeconfig.h"
#include "neutube.h"
#include "ui_zdviddialog.h"
#include "dvid/zdvidtarget.h"
#include "zjsonarray.h"
#include "zjsonobject.h"
#include "zdialogfactory.h"
#include "stringlistdialog.h"

const char* ZDvidDialog::m_dvidRepoKey = "dvid repo";

ZDvidDialog::ZDvidDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ZDvidDialog)
{
  ui->setupUi(this);
  ZDvidTarget customTarget("emdata1.int.janelia.org", "", -1);
  customTarget.setName("Custom");
  m_dvidRepo.push_back(customTarget);

  QRegExp rx("[^\\s]*");
  QValidator *validator = new QRegExpValidator(rx, this);
  ui->labelBlockLineEdit->setValidator(validator);

#if defined(_FLYEM_)
  const std::vector<ZDvidTarget> dvidRepo = GET_FLYEM_CONFIG.getDvidRepo();

  std::string userName = NeuTube::GetCurrentUserName();

  for (std::vector<ZDvidTarget>::const_iterator iter = dvidRepo.begin();
           iter != dvidRepo.end(); ++iter) {
    const ZDvidTarget &target = *iter;
    bool access = true;
    if (!target.getUserNameSet().empty()) {
      if (target.getUserNameSet().count(userName) == 0) {
        access = false;
      }
    }
    if (access) {
      m_dvidRepo.push_back(target);
    }
  }

  //Load locally saved targets
  QSettings &settings = NeutubeConfig::getInstance().getSettings();
  if (settings.contains("DVID")) {
    QString localDvidJsonStr = settings.value("DVID").toString();
    if (!localDvidJsonStr.isEmpty()) {
      ZJsonArray localDvidJson;
      localDvidJson.decode(localDvidJsonStr.toStdString());
      for (size_t i = 0; i < localDvidJson.size(); ++i) {
        ZJsonObject dvidTargetJson(
              localDvidJson.at(i), ZJsonValue::SET_INCREASE_REF_COUNT);
        ZDvidTarget target;
        target.loadJsonObject(dvidTargetJson);
        m_dvidRepo.push_back(target);
      }
    }
  }

//  m_dvidRepo.insert(m_dvidRepo.end(), dvidRepo.begin(), dvidRepo.end());
#endif

  m_roiDlg = new StringListDialog(this);

  for (QList<ZDvidTarget>::const_iterator iter = m_dvidRepo.begin();
       iter != m_dvidRepo.end(); ++iter) {
    const ZDvidTarget &target = *iter;
    if (!target.getName().empty()) {
      ui->serverComboBox->addItem(target.getName().c_str());
    } else {
      ui->serverComboBox->addItem(target.getSourceString(false).c_str());
    }
  }

  setServer(0);
  connect(ui->serverComboBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(setServer(int)));

  connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveCurrentTarget()));
  connect(ui->saveAsButton, SIGNAL(clicked()), this, SLOT(saveCurrentTargetAs()));
  connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteCurrentTarget()));
  connect(ui->roiPushButton, SIGNAL(clicked()), this, SLOT(editRoiList()));

//  setFixedSize(size());

  ui->roiLabel->hide();
  ui->roiPushButton->hide();
}

ZDvidDialog::~ZDvidDialog()
{
  delete ui;
}

int ZDvidDialog::getPort() const
{
  return ui->portSpinBox->value();
}

QString ZDvidDialog::getAddress() const
{
  return ui->addressLineEdit->text();
}

QString ZDvidDialog::getUuid() const
{
  return ui->uuidLineEdit->text();
}

ZDvidTarget &ZDvidDialog::getDvidTarget()
{
  ZDvidTarget &target = m_dvidRepo[ui->serverComboBox->currentIndex()];
  if (target.isEditable()) {
    target.setServer(getAddress().toStdString());
    target.setUuid(getUuid().toStdString());
    target.setPort(getPort());
    if (ui->bodyLineEdit->text().isEmpty()) {
      target.setNullBodyLabelName();
    } else {
      target.setBodyLabelName(ui->bodyLineEdit->text().toStdString());
    }
    if (ui->labelBlockLineEdit->text().isEmpty()) {
      target.setNullLabelBlockName();
    } else {
      target.setLabelBlockName(ui->labelBlockLineEdit->text().toStdString());
    }
    target.setGrayScaleName(ui->grayScalelineEdit->text().toStdString());

    std::string tileName = ui->tileLineEdit->text().toStdString();
    target.setMultiscale2dName(tileName);
    target.configTile(tileName, ui->lowQualityCheckBox->isChecked());

    target.setSynapseName(ui->synapseLineEdit->text().toStdString());
    target.enableSupervisor(ui->librarianCheckBox->isChecked());
    target.setSupervisorServer(ui->librarianLineEdit->text().toStdString());
    target.setMaxLabelZoom(ui->maxZoomSpinBox->value());
    target.setRoiName(ui->roiLineEdit->text().toStdString());
//    target.setLabelszName(ui->labelszLineEdit->text().toStdString());
//    target.setSupervisorServer(ui->liblineEdit->text().toStdString());
  }

  return target;
  /*
  ZDvidTarget target(
        getAddress().toStdString(), getUuid().toStdString(), getPort());
  target.setName(
        ui->serverComboBox->itemText(ui->serverComboBox->currentIndex()).
        toStdString());
  target.setComment(ui->infoLabel->text().toStdString());

  return target;
  */
}

void ZDvidDialog::setServer(int index)
{
  ZDvidTarget dvidTarget = m_dvidRepo[index];

  ui->addressLineEdit->setText(dvidTarget.getAddress().c_str());
  ui->portSpinBox->setValue(dvidTarget.getPort());
  ui->uuidLineEdit->setText(dvidTarget.getUuid().c_str());
  ui->infoLabel->setText(dvidTarget.getComment().c_str());
  ui->bodyLineEdit->setText(dvidTarget.getBodyLabelName().c_str());
  ui->grayScalelineEdit->setText(dvidTarget.getGrayScaleName().c_str());
  ui->labelBlockLineEdit->setText(dvidTarget.getLabelBlockName().c_str());
  ui->maxZoomSpinBox->setValue(dvidTarget.getMaxLabelZoom());
//  ui->labelszLineEdit->setText(dvidTarget.getLabelszName().c_str());
  ui->tileLineEdit->setText(dvidTarget.getMultiscale2dName().c_str());
  if (index == 0) {
    ui->lowQualityCheckBox->setChecked(false);
  } else {
    ui->lowQualityCheckBox->setChecked(
          dvidTarget.isLowQualityTile(dvidTarget.getMultiscale2dName()));
  }
  ui->synapseLineEdit->setText(dvidTarget.getSynapseName().c_str());
  ui->librarianCheckBox->setChecked(dvidTarget.isSupervised());
#if defined(_FLYEM_)
  ui->librarianLineEdit->setText(
        dvidTarget.getSupervisor().empty() ?
        GET_FLYEM_CONFIG.getDefaultLibrarian().c_str() :
        dvidTarget.getSupervisor().c_str());
#endif
  ui->roiLineEdit->setText(dvidTarget.getRoiName().c_str());

  ui->addressLineEdit->setReadOnly(!dvidTarget.isEditable());
  ui->portSpinBox->setReadOnly(!dvidTarget.isEditable());
  ui->uuidLineEdit->setReadOnly(!dvidTarget.isEditable());
  ui->bodyLineEdit->setReadOnly(!dvidTarget.isEditable());
  ui->labelBlockLineEdit->setReadOnly(!dvidTarget.isEditable());
  ui->grayScalelineEdit->setReadOnly(!dvidTarget.isEditable());
  ui->tileLineEdit->setReadOnly(!dvidTarget.isEditable());
  ui->synapseLineEdit->setReadOnly(!dvidTarget.isEditable());
  ui->librarianCheckBox->setEnabled(dvidTarget.isEditable());
  ui->librarianLineEdit->setReadOnly(!dvidTarget.isEditable());
  ui->maxZoomSpinBox->setReadOnly(!dvidTarget.isEditable());
  ui->roiLineEdit->setReadOnly(!dvidTarget.isEditable());
//  ui->labelszLineEdit->setReadOnly(!dvidTarget.isEditable());

  ui->saveButton->setEnabled(dvidTarget.isEditable());
  ui->deleteButton->setEnabled(dvidTarget.isEditable() &&
                               (dvidTarget.getName() != "Custom"));
  ui->roiLabel->setText(QString("%1 ROI").arg(dvidTarget.getRoiList().size()));

}

bool ZDvidDialog::hasNameConflict(const std::string &name) const
{
  for (QList<ZDvidTarget>::const_iterator iter = m_dvidRepo.begin();
       iter != m_dvidRepo.end(); ++iter) {
    const ZDvidTarget &target = *iter;
    if (name == target.getName()) {
      return true;
    }
  }

  return false;
}

void ZDvidDialog::addDvidTarget(ZDvidTarget &target)
{
  if (hasNameConflict(target.getName()) || target.getName().empty()) {
    if (ZDialogFactory::Ask(
          "Invalid Name",
          QString("Empty name or %1 alread exists. "
                  "Use a different name?").arg(target.getName().c_str()),
          this)) {
      QString targetName =
          QInputDialog::getText(this, "Database Name", "Database Name");
      target.setName(targetName.toStdString());
      addDvidTarget(target);
    }
  } else {
    m_dvidRepo.push_back(target);
    ui->serverComboBox->addItem(target.getName().c_str());
  }
}

void ZDvidDialog::saveCurrentTarget()
{
  saveCurrentTarget(false);
}

void ZDvidDialog::saveCurrentTargetAs()
{
  saveCurrentTarget(true);
}

void ZDvidDialog::saveCurrentTarget(bool cloning)
{
  ZDvidTarget target = getDvidTarget();
  if (target.getName() == "Custom") {
    cloning = true;
  }
  bool cloned = false;
  if (cloning) {
    QString targetName =
        QInputDialog::getText(this, "Database Name", "Database Name");
    if (!targetName.isEmpty()) {
      target.setName(targetName.toStdString());
      target.setEditable(true);
      addDvidTarget(target);
      ui->serverComboBox->setCurrentIndex(ui->serverComboBox->count() - 1);
//      setServer(ui->serverComboBox->count() - 1);
      cloned = true;
    }
  }

  if (!cloning || cloned) {
    QSettings &settings = NeutubeConfig::getInstance().getSettings();

    ZJsonArray dvidJson;
    dvidJson.decode(settings.value("DVID").toString().toStdString());
    if (cloned) {
      dvidJson.append(target.toJsonObject());
    } else {
      for (size_t i = 0; i < dvidJson.size(); ++i) {
        ZJsonObject dvidTargetJson(dvidJson.at(i), ZJsonValue::SET_INCREASE_REF_COUNT);
        ZDvidTarget tmpTarget;
        tmpTarget.loadJsonObject(dvidTargetJson);
        if (tmpTarget.getName() == target.getName()) {
          dvidJson.setValue(i, target.toJsonObject());
          break;
        }
      }
      m_dvidRepo[ui->serverComboBox->currentIndex()] = target;
    }

#ifdef _DEBUG_
    std::cout << dvidJson.dumpString(0) << std::endl;
#endif
    settings.setValue("DVID", QString(dvidJson.dumpString(0).c_str()));
  }
}

void ZDvidDialog::deleteCurrentTarget()
{
  ZDvidTarget target = getDvidTarget();
  if (target.isEditable() && target.getName() != "Custom") {
    QSettings &settings = NeutubeConfig::getInstance().getSettings();

    ZJsonArray dvidJson;
    dvidJson.decode(settings.value("DVID").toString().toStdString());
    for (size_t i = 0; i < dvidJson.size(); ++i) {
      ZJsonObject dvidTargetJson(dvidJson.at(i), ZJsonValue::SET_INCREASE_REF_COUNT);
      ZDvidTarget tmpTarget;
      tmpTarget.loadJsonObject(dvidTargetJson);
      if (tmpTarget.getName() == target.getName()) {
        dvidJson.remove(i);
        break;
      }
    }

    settings.setValue("DVID", QString(dvidJson.dumpString(0).c_str()));

    m_dvidRepo.removeAt(ui->serverComboBox->currentIndex());
    ui->serverComboBox->removeItem(ui->serverComboBox->currentIndex());
  }
}

void ZDvidDialog::editRoiList()
{
  m_roiDlg->setStringList(getDvidTarget().getRoiList());
  if (m_roiDlg->exec()) {
    QStringList strList = m_roiDlg->getStringList();
    std::vector<std::string> roiList;
    foreach(const QString &str, strList) {
      roiList.push_back(str.toStdString());
    }

    getDvidTarget().setRoiList(roiList);
    ui->roiLabel->setText(QString("%1 ROI").arg(roiList.size()));
  }
}
