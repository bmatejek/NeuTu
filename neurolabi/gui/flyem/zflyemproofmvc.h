#ifndef ZFLYEMPROOFMVC_H
#define ZFLYEMPROOFMVC_H

#include <vector>
#include <QString>
#include <QMetaType>
#include <QSharedPointer>
#include <QMap>

#include "neutube_def.h"
#include "zstackmvc.h"
#include "flyem/zflyembodysplitproject.h"
#include "flyem/zflyembodymergeproject.h"
#include "zthreadfuturemap.h"
#include "flyem/zflyembookmark.h"
#include "zwindowfactory.h"
#include "flyem/zflyembody3ddoc.h"

class QWidget;
class ZFlyEmProofDoc;
class ZDvidTileEnsemble;
class ZDvidTarget;
class ZDvidDialog;
class ZFlyEmProofPresenter;
class ZFlyEmSupervisor;
class ZPaintLabelWidget;
class FlyEmBodyInfoDialog;
class ProtocolSwitcher;
class ZFlyEmSplitCommitDialog;
class ZFlyEmOrthoWindow;
class ZFlyEmDataFrame;
class FlyEmTodoDialog;
class ZClickableColorLabel;
class ZColorLabel;
class ZFlyEmSynapseDataFetcher;
class ZFlyEmSynapseDataUpdater;
class ZDvidPatchDataFetcher;
class ZDvidPatchDataUpdater;
class ZFlyEmRoiToolDialog;
class QSortFilterProxyModel;
class ZFlyEmBookmarkView;

/*!
 * \brief The MVC class for flyem proofreading
 */
class ZFlyEmProofMvc : public ZStackMvc
{
  Q_OBJECT
public:
  explicit ZFlyEmProofMvc(QWidget *parent = 0);
  ~ZFlyEmProofMvc();

  static ZFlyEmProofMvc* Make(
      QWidget *parent, ZSharedPointer<ZFlyEmProofDoc> doc,
      NeuTube::EAxis axis = NeuTube::Z_AXIS);
  static ZFlyEmProofMvc* Make(const ZDvidTarget &target);

  ZFlyEmProofDoc* getCompleteDocument() const;
  ZFlyEmProofPresenter* getCompletePresenter() const;

  template <typename T>
  void connectControlPanel(T *panel);

  template <typename T>
  void connectSplitControlPanel(T *panel);

  ZDvidTileEnsemble* getDvidTileEnsemble();

  virtual void setDvidTarget(const ZDvidTarget &target);
  void setDvidTargetFromDialog();

  void clear();

  void exitCurrentDoc();

  void enableSplit();
  void disableSplit();

  void processViewChangeCustom(const ZStackViewParam &viewParam);

  ZFlyEmSupervisor* getSupervisor() const;

//  bool checkInBody(uint64_t bodyId);
  bool checkOutBody(uint64_t bodyId);

  virtual ZDvidTarget getDvidTarget() const;

  void setDvidDialog(ZDvidDialog *dlg);

  ZFlyEmBodyMergeProject* getMergeProject() {
    return &m_mergeProject;
  }

//  ZFlyEmBookmarkListModel* getAssignedBookmarkModel() const;
//  ZFlyEmBookmarkListModel* getUserBookmarkModel() const;
//  QSortFilterProxyModel* getAssignedBookmarkProxy() const;
//  QSortFilterProxyModel* getUserBookmarkProxy() const;

  ZFlyEmBookmarkListModel* getAssignedBookmarkModel(
     FlyEM::EProofreadingMode mode) const {
    return m_assignedBookmarkModel[mode];
  }

  ZFlyEmBookmarkListModel* getUserBookmarkModel(
     FlyEM::EProofreadingMode mode) const {
    return m_userBookmarkModel[mode];
  }

  ZFlyEmBookmarkListModel* getUserBookmarkModel() const;
  ZFlyEmBookmarkListModel* getAssignedBookmarkModel() const;

  void registerBookmarkView(ZFlyEmBookmarkView *view);

signals:
  void launchingSplit(const QString &message);
  void launchingSplit(uint64_t bodyId);
  void messageGenerated(const QString &message, bool appending = true);
  void errorGenerated(const QString &message, bool appending = true);
  void messageGenerated(const ZWidgetMessage &message);
  void splitBodyLoaded(uint64_t bodyId);
  void bookmarkUpdated(ZFlyEmBodyMergeProject *m_project);
  void bookmarkUpdated(ZFlyEmBodySplitProject *m_project);
  void bookmarkDeleted(ZFlyEmBodyMergeProject *m_project);
  void bookmarkDeleted(ZFlyEmBodySplitProject *m_project);
  void dvidTargetChanged(ZDvidTarget);
  void userBookmarkUpdated(ZStackDoc *doc);
  void nameColorMapReady(bool ready);
  void bodyMergeEdited();
  void updatingLatency(int);
//  void highlightModeEnabled(bool);
  void highlightModeChanged();
  void roiLoaded();


public slots:
  void mergeSelected();
  void unmergeSelected();
  void undo();
  void redo();

  void setSegmentationVisible(bool visible);
  void setDvidTarget();
  void launchSplit(uint64_t bodyId);
  void processMessageSlot(const QString &message);
  void notifySplitTriggered();
  void annotateBody();
  void annotateSynapse();
  void checkInSelectedBody();
  void checkInSelectedBodyAdmin();
  void checkOutBody();
  bool checkInBody(uint64_t bodyId);
  bool checkInBodyWithMessage(uint64_t bodyId);
  bool checkBodyWithMessage(uint64_t bodyId, bool checkingOut);
  void exitSplit();
  void switchSplitBody(uint64_t bodyId);
  void showBodyQuickView();
  void showSplitQuickView();
  void presentBodySplit(uint64_t bodyId);
  void updateBodySelection();
  void saveSeed();
  void saveMergeOperation();
  void commitMerge();
  void commitCurrentSplit();
  void locateBody(uint64_t bodyId, bool appending);
  void locateBody(uint64_t bodyId);
  void locateBody(QList<uint64_t> bodyIdList);
  void addLocateBody(uint64_t bodyId);
  void selectBody(uint64_t bodyId);
  void selectBodyInRoi(bool appending = true);
  void selectBody(QList<uint64_t> bodyIdList);

  void showBody3d();
  void showSplit3d();
  void showCoarseBody3d();
  void showFineBody3d();
  void showSkeletonWindow();
  void showExternalNeuronWindow();
  void showObjectWindow();
  void showRoi3dWindow();
  void showQueryTable();
  void showOrthoWindow(double x, double y, double z);

  void setDvidLabelSliceSize(int width, int height);
  void showFullSegmentation();

  void enhanceTileContrast(bool state);
  void smoothDisplay(bool state);

  void goToBody();
  void goToBodyBottom();
  void goToBodyTop();
  void selectBody();
  void processLabelSliceSelectionChange();
  void decomposeBody();

  void loadBookmark(const QString &filePath);
  void addSelectionAt(int x, int y, int z);
  void xorSelectionAt(int x, int y, int z);
  void deselectAllBody();
  void selectSeed();
  void setMainSeed();
  void selectAllSeed();
  void recoverSeed();
  void exportSeed();
  void importSeed();
  void runSplit();
  void runLocalSplit();

  void loadSynapse();
  void goToTBar();
  void showSynapseAnnotation(bool visible);
  void showBookmark(bool visible);
  void showSegmentation(bool visible);
  void showRoiMask(bool visible);
  void showData(bool visible);
  void setHighContrast(bool on);
  void toggleSegmentation();
  void showTodo(bool visible);
  void toggleData();

  void loadBookmark();
  void openSequencer();
  void openProtocol();
  void openTodo();
  void openRoiTool();

  void goToNearestRoi();
  void loadRoiProject();
  void closeRoiProject();
  void updateRoiGlyph();
  void estimateRoi();

  void checkSelectedBookmark(bool checking);
  void recordCheckedBookmark(const QString &key, bool checking);
  void recordBookmark(ZFlyEmBookmark *bookmark);
  void processSelectionChange(const ZStackObjectSelector &selector);

  void annotateBookmark(ZFlyEmBookmark *bookmark);

  void updateBookmarkTable();
  void updateUserBookmarkTable();
  void updateAssignedBookmarkTable();
  void appendAssignedBookmarkTable(const QList<ZFlyEmBookmark*> &bookmarkList);
  void appendUserBookmarkTable(const QList<ZFlyEmBookmark*> &bookmarkList);
//  void updateAssignedBookmarkTable();
  void sortUserBookmarkTable();
  void sortAssignedBookmarkTable();

  void processCheckedUserBookmark(ZFlyEmBookmark *bookmark);

  void changeColorMap(const QString &option);

  void removeLocalBookmark(ZFlyEmBookmark *bookmark);
  void addLocalBookmark(ZFlyEmBookmark *bookmark);

  void removeBookmark(ZFlyEmBookmark *bookmark);
  void removeBookmark(const QList<ZFlyEmBookmark*> &bookmarkList);

  void highlightSelectedObject(ZDvidLabelSlice *labelSlice, bool hl);
  void highlightSelectedObject(bool hl);

  void syncMergeWithDvid();

  void getROIs();
  void updateLatencyWidget(int t);

  void suppressObjectVisible();
  void recoverObjectVisible();

  void updateRoiWidget();
//  void toggleEdgeMode(bool edgeOn);

protected slots:
  void detachCoarseBodyWindow();
  void detachBodyWindow();
  void detachSplitWindow();
  void detachSkeletonWindow();
  void detachObjectWindow();
  void detachRoiWindow();
  void detachExternalNeuronWindow();
  void detachOrthoWindow();
  void detachQueryWindow();
//  void closeBodyWindow(int index);
  void closeOrthoWindow();
  void close3DWindow(Z3DWindow *window);
  void closeBodyWindow(Z3DWindow *window);
  void closeAllBodyWindow();
  void closeAllAssociatedWindow();
  void updateCoarseBodyWindow();
  void updateCoarseBodyWindowDeep();
  void updateBodyWindow();
  void updateBodyWindowDeep();
  void updateSkeletonWindow();
  void cropCoarseBody3D();
  void showBodyGrayscale();
  void updateSplitBody();
  void updateBodyMerge();
  void updateCoarseBodyWindowColor();
  void prepareBodyMap(const ZJsonValue &bodyInfoObj);
  void clearBodyMergeStage();
  void exportSelectedBody();
  void processSynapseVerification(int x, int y, int z, bool verified);
  void processSynapseMoving(const ZIntPoint &from, const ZIntPoint &to);
//  void notifyBookmarkDeleted();

protected:
  void customInit();
  void createPresenter();
  virtual void dropEvent(QDropEvent *event);
  void enableSynapseFetcher();

private:
  void init();
  void initBodyWindow();
  void launchSplitFunc(uint64_t bodyId);
  uint64_t getMappedBodyId(uint64_t bodyId);
  std::set<uint64_t> getCurrentSelectedBodyId(NeuTube::EBodyLabelType type) const;
  void runSplitFunc();
  void runLocalSplitFunc();
//  void notifyBookmarkUpdated();

  void syncDvidBookmark();
  void loadBookmarkFunc(const QString &filePath);
  void loadROIFunc();

  void makeCoarseBodyWindow();
  void makeBodyWindow();
  void makeSkeletonWindow();
  void makeSplitWindow();
  void makeExternalNeuronWindow();
  void makeOrthoWindow();

  ZFlyEmBody3dDoc *makeBodyDoc(ZFlyEmBody3dDoc::EBodyType bodyType);

  void mergeCoarseBodyWindow();

  void updateCoarseBodyWindow(bool showingWindow, bool resettingCamera,
                              bool isDeep);
  void updateBodyWindowForSplit();

  void setWindowSignalSlot(Z3DWindow *window);
  void updateBodyWindowPlane(
      Z3DWindow *window, const ZStackViewParam &viewParam);
  ZDvidLabelSlice* getDvidLabelSlice() const;

  void clearAssignedBookmarkModel();
  void clearUserBookmarkModel();
//  void prepareBookmarkModel(ZFlyEmBookmarkListModel *model,
//                            QSortFilterProxyModel *proxy);

protected:
  bool m_showSegmentation;
  ZFlyEmBodySplitProject m_splitProject;
  ZFlyEmBodyMergeProject m_mergeProject;

  QMap<FlyEM::EProofreadingMode, ZFlyEmBookmarkListModel*>
  m_assignedBookmarkModel;
  QMap<FlyEM::EProofreadingMode, ZFlyEmBookmarkListModel*>
  m_userBookmarkModel;

//  ZFlyEmBookmarkListModel *m_assignedBookmarkList;
//  ZFlyEmBookmarkListModel *m_userBookmarkList;

//  QSortFilterProxyModel *m_assignedBookmarkProxy;
//  QSortFilterProxyModel *m_userBookmarkProxy;
//  ZFlyEmBookmarkArray m_bookmarkArray;

  ZThreadFutureMap m_futureMap;

//  ZColorLabel *m_latencyLabelWidget;
  ZPaintLabelWidget *m_paintLabelWidget;

  ZDvidDialog *m_dvidDlg;
  FlyEmBodyInfoDialog *m_bodyInfoDlg;
  ProtocolSwitcher *m_protocolSwitcher;
  ZFlyEmSplitCommitDialog *m_splitCommitDlg;
  FlyEmTodoDialog *m_todoDlg;
  ZFlyEmRoiToolDialog *m_roiDlg;

  Z3DMainWindow *m_bodyViewWindow;
  Z3DTabWidget *m_bodyViewers;
  Z3DWindow *m_coarseBodyWindow;
  Z3DWindow *m_bodyWindow;
  Z3DWindow *m_skeletonWindow;
  Z3DWindow *m_externalNeuronWindow;
  Z3DWindow *m_splitWindow;
  Z3DWindow *m_objectWindow;
  Z3DWindow *m_roiWindow;
  ZFlyEmOrthoWindow *m_orthoWindow;
//  ZFlyEmDataFrame *m_queryWindow;
  QSharedPointer<ZWindowFactory> m_bodyWindowFactory;

  ZStackViewParam m_currentViewParam;

  ZDvidInfo m_dvidInfo;
  bool m_ROILoaded;

  std::vector<std::string> m_roiList;
  std::vector<ZObject3dScan> m_loadedROIs;
  std::vector<std::string> m_roiSourceList;

  //Data fetching
  ZFlyEmSynapseDataFetcher *m_seFetcher;
  ZFlyEmSynapseDataUpdater *m_seUpdater;
//  ZDvidPatchDataFetcher *m_patchFetcher;
//  ZDvidPatchDataUpdater *m_patchUpdater;
};

template <typename T>
void ZFlyEmProofMvc::connectControlPanel(T *panel)
{
  connect(panel, SIGNAL(segmentVisibleChanged(bool)),
          this, SLOT(setSegmentationVisible(bool)));
  connect(panel, SIGNAL(mergingSelected()), this, SLOT(mergeSelected()));
//  connect(panel, SIGNAL(edgeModeToggled(bool)),
//          this, SLOT(toggleEdgeMode(bool)));
  connect(panel, SIGNAL(dvidSetTriggered()), this, SLOT(setDvidTarget()));
  connect(this, SIGNAL(dvidTargetChanged(ZDvidTarget)),
          panel, SLOT(setDvidInfo(ZDvidTarget)));
  connect(this, SIGNAL(launchingSplit(uint64_t)),
          panel, SIGNAL(splitTriggered(uint64_t)));
  connect(panel, SIGNAL(labelSizeChanged(int, int)),
          this, SLOT(setDvidLabelSliceSize(int, int)));
  connect(panel, SIGNAL(showingFullSegmentation()),
          this, SLOT(showFullSegmentation()));
  connect(panel, SIGNAL(coarseBodyViewTriggered()),
          this, SLOT(showCoarseBody3d()));
  connect(panel, SIGNAL(bodyViewTriggered()),
          this, SLOT(showFineBody3d()));
  connect(panel, SIGNAL(skeletonViewTriggered()),
          this, SLOT(showSkeletonWindow()));
  connect(panel, SIGNAL(savingMerge()), this, SLOT(saveMergeOperation()));
  connect(panel, SIGNAL(committingMerge()), this, SLOT(commitMerge()));
  connect(panel, SIGNAL(zoomingTo(int, int, int)),
          this, SLOT(zoomTo(int, int, int)));
  connect(panel, SIGNAL(locatingBody(uint64_t)),
          this, SLOT(locateBody(uint64_t)));
  connect(panel, SIGNAL(goingToBody()), this, SLOT(goToBody()));
  connect(panel, SIGNAL(selectingBody()), this, SLOT(selectBody()));
//  connect(this, SIGNAL(bookmarkUpdated(ZFlyEmBodyMergeProject*)),
//          panel, SLOT(updateBookmarkTable(ZFlyEmBodyMergeProject*)));
//  connect(this, SIGNAL(bookmarkDeleted(ZFlyEmBodyMergeProject*)),
//          panel, SLOT(clearBookmarkTable(ZFlyEmBodyMergeProject*)));
//  connect(panel, SIGNAL(bookmarkChecked(QString, bool)),
//          this, SLOT(recordCheckedBookmark(QString, bool)));
//  connect(panel, SIGNAL(bookmarkChecked(ZFlyEmBookmark*)),
//          this, SLOT(recordBookmark(ZFlyEmBookmark*)));
//  connect(this, SIGNAL(userBookmarkUpdated(ZStackDoc*)),
//          panel, SLOT(updateUserBookmarkTable(ZStackDoc*)));
//  connect(panel, SIGNAL(userBookmarkChecked(ZFlyEmBookmark*)),
//          this, SLOT(processCheckedUserBookmark(ZFlyEmBookmark*)));
//  connect(panel, SIGNAL(removingBookmark(ZFlyEmBookmark*)),
//          this, SLOT(removeBookmark(ZFlyEmBookmark*)));
//  connect(panel, SIGNAL(removingBookmark(QList<ZFlyEmBookmark*>)),
//          this, SLOT(removeBookmark(QList<ZFlyEmBookmark*>)));
  connect(panel, SIGNAL(changingColorMap(QString)),
          this, SLOT(changeColorMap(QString)));
  connect(this, SIGNAL(nameColorMapReady(bool)),
          panel, SLOT(enableNameColorMap(bool)));
  connect(panel, SIGNAL(clearingBodyMergeStage()),
          this, SLOT(clearBodyMergeStage()));
  connect(panel, SIGNAL(exportingSelectedBody()),
          this, SLOT(exportSelectedBody()));
  connect(this, SIGNAL(updatingLatency(int)), panel, SLOT(updateLatency(int)));
}

template <typename T>
void ZFlyEmProofMvc::connectSplitControlPanel(T *panel)
{
  connect(panel, SIGNAL(quickViewTriggered()), this, SLOT(showBodyQuickView()));
  connect(panel, SIGNAL(coarseBodyViewTriggered()),
          this, SLOT(showCoarseBody3d()));
  connect(panel, SIGNAL(splitQuickViewTriggered()),
          this, SLOT(showSplitQuickView()));
  connect(panel, SIGNAL(bodyViewTriggered()), this, SLOT(showBody3d()));
  connect(panel, SIGNAL(splitViewTriggered()), this, SLOT(showSplit3d()));

  connect(panel, SIGNAL(exitingSplit()), this, SLOT(exitSplit()));
  connect(panel, SIGNAL(changingSplit(uint64_t)), this,
          SLOT(switchSplitBody(uint64_t)));
  connect(panel, SIGNAL(savingSeed()), this, SLOT(saveSeed()));
  connect(panel, SIGNAL(committingResult()), this, SLOT(commitCurrentSplit()));
  connect(panel, SIGNAL(loadingBookmark(QString)),
          this, SLOT(loadBookmark(QString)));
//  connect(this, SIGNAL(bookmarkUpdated(ZFlyEmBodySplitProject*)),
//          panel, SLOT(updateBookmarkTable(ZFlyEmBodySplitProject*)));
//  connect(this, SIGNAL(bookmarkDeleted(ZFlyEmBodySplitProject*)),
//          panel, SLOT(clearBookmarkTable(ZFlyEmBodySplitProject*)));
//  connect(this, SIGNAL(userBookmarkUpdated(ZStackDoc*)),
//          panel, SLOT(updateUserBookmarkTable(ZStackDoc*)));
  connect(panel, SIGNAL(zoomingTo(int, int, int)),
          this, SLOT(zoomTo(int, int, int)));
  connect(panel, SIGNAL(settingMainSeed()), this, SLOT(setMainSeed()));
  connect(panel, SIGNAL(selectingSeed()), this, SLOT(selectSeed()));
  connect(panel, SIGNAL(selectingAllSeed()), this, SLOT(selectAllSeed()));
  connect(panel, SIGNAL(recoveringSeed()), this, SLOT(recoverSeed()));
  connect(panel, SIGNAL(exportingSeed()), this, SLOT(exportSeed()));
  connect(panel, SIGNAL(importingSeed()), this, SLOT(importSeed()));
  connect(this, SIGNAL(splitBodyLoaded(uint64_t)),
          panel, SLOT(updateBodyWidget(uint64_t)));
  connect(panel, SIGNAL(loadingSynapse()), this, SLOT(loadSynapse()));
  connect(panel, SIGNAL(bookmarkChecked(QString, bool)),
          this, SLOT(recordCheckedBookmark(QString, bool)));
  connect(panel, SIGNAL(bookmarkChecked(ZFlyEmBookmark*)),
          this, SLOT(recordBookmark(ZFlyEmBookmark*)));
  connect(panel, SIGNAL(croppingCoarseBody3D()), this, SLOT(cropCoarseBody3D()));
  connect(panel, SIGNAL(showingBodyGrayscale()), this, SLOT(showBodyGrayscale()));
}


#endif // ZFLYEMPROOFMVC_H
