#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QtWidgets\qmainwindow.h>
#include <QtWidgets\qactiongroup.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qstatusbar.h>
#include <QtWidgets\qmenubar.h>
#include <QtWidgets\qfiledialog.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qslider.h>
#include <QtWidgets\QSpacerItem>
#include <QtWidgets\qprogressbar.h>
#include <QtWidgets\qradiobutton.h>
#include <QtWidgets\qscrollarea.h>
#include <QtWidgets\qinputdialog.h>

#include "myGLWindow.h"
#include "myDataManager.h"
#include "mesh/neighborhood.h"
#include "smoothing.h"

enum globalSmoothingStatus { gs_status_init, gs_status_started, gs_status_stopping, gs_status_stopped, gs_status_continuing };

enum globalSmoothingAlgorithm {gs_algorithm_bilateral_normal , gs_algorithm_guided};
const vector<string> globalSmoothingAlgorithmLabels = {"Bilateral Normal Filtering","Guided Mesh Denoising"};

enum focalizedSmoothingAlgorithm {fs_algorithm_uniform_laplacian, fs_algorithm_hc_laplacian};
const vector<string> focalizedSmoothingAlgorithmLabels = { "Uniform Laplacian", "HC Laplacian" };

class myMainWindow : public QMainWindow
{
	//Q_OBJECT
	myGLWindow * input_mesh_visualizer_ptr;
	myGLWindow * output_mesh_visualizer_ptr;

	QWidget * widget;
	QHBoxLayout * layout;
	QScrollArea * control_scroll_area;
	QVBoxLayout * control_layout;

	QGroupBox * group_box_smoothing_type;
	QGroupBox * group_box_global_smoothing;
	QGroupBox * group_box_focalized_smoothing;
	QGroupBox * group_box_data_manipulation;

	QHBoxLayout * layout_smoothing_type;
	QVBoxLayout *layout_global_smoothing;
	QHBoxLayout * layout_global_smoothing_buttons;
	QVBoxLayout * layout_focalized_smoothing;
	QHBoxLayout * layout_data_manipulation;

	QRadioButton * radio_button_smoothing_type_g;
	QRadioButton * radio_button_smoothing_type_f;

	QPushButton * push_button_global_smoothing_run;
	QPushButton * push_button_global_smoothing_stop;
	QPushButton * push_button_global_smoothing_continue;
	QPushButton * push_button_reinitialize_data;
	QPushButton * push_button_update_input_data;

	QSlider * slider_gs_smoothness;
	QSlider * slider_gs_radius_ratio;
	QSlider * slider_gs_detail_preservation;
	QSlider * slider_fs_smoothness;
	QSlider * slider_fs_radius;

	QProgressBar * progress_bar;

	//////////////

	QMenu * fileMenu;
	QMenu * viewMenu;
	QMenu * settingsMenu;
	QMenu * helpMenu;
	QActionGroup * alignmentGroup;
	QAction * loadAct;
	QAction * saveAct;
	QAction * exitAct;
	QAction * flatModeAct;
	QAction * wireframeModeAct;
	QAction * pointsModeAct;
	QAction * setShadersAct;
	QAction * setGlobalSmoothingAlgorithmAct;
	QAction * setFocalizedSmoothingAlgorithmAct;
	QAction * aboutAct;
	QAction * userGuideAct;

	//////////////

	myDataManager data;
	QThread smoothingThread;
	GlobalSmoothingTask * smoothingTask;

	globalSmoothingAlgorithm currentGlobalSmoothingAlgorithm;
	focalizedSmoothingAlgorithm currentFocalizedSmoothingAlgorithm;

	bool selectionMode;  //   true if we are pressing shift
	bool selectionStatus; //   not running: 0          running: 1     
	bool runningStatus; //   not running: 0          running: 1     

	bool globalSmoothingStopped;  // true if global smoothing was stopped

public:
	myMainWindow();
	~myMainWindow();
protected:
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
	bool eventFilter(QObject *object, QEvent *event);
private:
	void loadMesh();
	void saveMesh();
	void exit();
	void flatMode();
	void wireframeMode();
	void pointsMode();
	void setShaders();
	void setGlobalSmoothingAlgorithm();
	void setFocalizedSmoothingAlgorithm();
	void about();
	void userGuide();
	void runGlobalSmoothing();
	void enableSmoothingType();
	void reinitializeOutput();
	void setOutputAsInput();

	void getSelection(vector<size_t> & selected_vertices_ids);
	void updateSelection();
	void removeSelection();
	void stopGlobalSmoothing();
	void continueGlobalSmoothing();
	void selectAndSmooth();

	void updateGlobalSmoothing();
	void setGlobalSmoothingStatus(globalSmoothingStatus current_status);

	void createActions();
	void createMenus();
	void updateWidgetValues();
	void setSmoothingThread();
};


#endif // MYMAINWINDOW_H