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

#include "myGLWindow.h"

class myMainWindow : public QMainWindow
{
	//Q_OBJECT

	myGLWindow * input_mesh_visualizer_ptr;
	myGLWindow * output_mesh_visualizer_ptr;

	QWidget *widget;
	QHBoxLayout *layout;
	QScrollArea * control_scroll_area;
	QVBoxLayout *control_layout;
	QGroupBox * group_box_smoothing_type;
	QGroupBox * group_box_global_smoothing;
	QGroupBox * group_box_focalized_smoothing;
	QHBoxLayout * layout_smoothing_type;
	QVBoxLayout *layout_global_smoothing;
	QHBoxLayout * layout_global_smoothing_buttons;
	QVBoxLayout * layout_focalized_smoothing;

	QRadioButton * radio_button_smoothing_type_g;
	QRadioButton * radio_button_smoothing_type_f;
	QPushButton * push_button_global_smoothing_run;
	QPushButton * push_button_global_smoothing_stop;
	QPushButton * push_button_global_smoothing_continue;
	QSlider * slider_gs_smoothness;
	QSlider * slider_gs_radius_ratio;
	QSlider * slider_gs_detail_preservation;
	QSlider * slider_fs_smoothness;
	QSlider * slider_fs_radius;
	QProgressBar * progress_bar;


public:
	myMainWindow();
	~myMainWindow();
protected:
private slots :
	void loadMesh();
	void saveMesh();
	void exit();
	void flatMode();
	void wireframeMode();
	void pointsMode();
	void setShaders();
	void setAlgorithms();
	void about();
	void userGuide();
	void runSmoothingGlobal();
	void enableSmoothingType();
private:
	void createActions();
	void createMenus();
	QMenu *fileMenu;
	QMenu *viewMenu;
	QMenu *settingsMenu;
	QMenu *helpMenu;
	QActionGroup *alignmentGroup;
	QAction *loadAct;
	QAction *saveAct;
	QAction *exitAct;
	QAction *flatModeAct;
	QAction *wireframeModeAct;
	QAction *pointsModeAct;
	QAction *setShadersAct;
	QAction *setAlgorithmsAct;
	QAction *aboutAct;
	QAction *userGuideAct;
	
};

#endif // MYMAINWINDOW_H