#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qactiongroup.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qstatusbar.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qslider.h>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/qprogressbar.h>
#include <QtWidgets/qradiobutton.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qinputdialog.h>
#include <QtWidgets/qmessagebox.h>
#include <QtGui/qdesktopservices.h>

#include "myGLWindow.h"
#include "myDataManager.h"
#include "mesh/neighborhood.h"
#include "smoothing.h"

/**
 * @brief The globalSmoothingStatus enum - global smoothing status
 */
enum globalSmoothingStatus {
    gs_status_init, /**< status init (global smoothing: not running and not started) */
    gs_status_started, /**< status started (global smoothing: running) */
    gs_status_stopping, /**< status stopping (global smoothing: running and will be stopped) */
    gs_status_stopped, /**< status stopped (global smoothing: not running) */
    gs_status_continuing /**< status continuing (global smoothing: running) */
};

/**
 * @brief The globalSmoothingAlgorithm enum - global smoothing algorithms
 */
enum globalSmoothingAlgorithm {
    gs_algorithm_bilateral_normal, /**< bilateral normal filtering */
    gs_algorithm_guided, /**< guided mesh denoising */
	gs_algorithm_uniform
};

///Global smoothing algorithm names
const vector<string> globalSmoothingAlgorithmLabels = {
    "Bilateral Normal Filtering",
    "Guided Mesh Denoising",
	"Uniform Laplacian"
};

/**
 * @brief The focalizedSmoothingAlgorithm enum - focalized smoothing algorithms
 */
enum focalizedSmoothingAlgorithm {
    fs_algorithm_uniform_laplacian, /**< uniform laplacian smoothing */
    fs_algorithm_hc_laplacian /**< hc laplacian smoothing */
};

///Focalized smoothing algorithm names
const vector<string> focalizedSmoothingAlgorithmLabels = {
    "Uniform Laplacian",
    "HC Laplacian"
};

/**
 * @brief The myMainWindow class - Main Window for mesh smoothing application
 */
class myMainWindow : public QMainWindow
{
    myGLWindow * input_mesh_visualizer_ptr; /**< visualization widget for input mesh */
    myGLWindow * output_mesh_visualizer_ptr; /**< visualization widget for output mesh */

    QWidget * widget; /**< main widget contained in main window */
    QHBoxLayout * layout; /**< main layout contained in main window */
    QScrollArea * control_scroll_area; /**< scroll area for control widgets */
    QVBoxLayout * control_layout; /**< layout for control widgets */

    QGroupBox * group_box_smoothing_type; /**< group box for smoothing type selection */
    QGroupBox * group_box_global_smoothing; /**< group box for global smoothing controls */
    QGroupBox * group_box_focalized_smoothing; /**< group box for focalized smoothing controls */
    QGroupBox * group_box_data_manipulation; /**< group box for data manipulation controls */

    ///Auxiliar Layouts
	QHBoxLayout * layout_smoothing_type;
	QVBoxLayout *layout_global_smoothing;
	QHBoxLayout * layout_global_smoothing_buttons;
	QVBoxLayout * layout_focalized_smoothing;
	QHBoxLayout * layout_data_manipulation;

    QRadioButton * radio_button_smoothing_type_g; /**< smoothing type selection radio button global smoothing */
    QRadioButton * radio_button_smoothing_type_f; /**< smoothing type selection radio button focalized smoothing */

    QPushButton * push_button_global_smoothing_run; /**< global smoothing run button */
    QPushButton * push_button_global_smoothing_stop; /**< global smoothing stop button */
    QPushButton * push_button_global_smoothing_continue; /**< global smoothing continue button */
    QPushButton * push_button_reinitialize_data; /**< reinitialize button */
    QPushButton * push_button_update_input_data; /**< update button */

    QSlider * slider_gs_smoothness; /**< global smoothing smoothness slider */
    QSlider * slider_gs_radius_ratio; /**< global smoothing radius ratio slider */
    QSlider * slider_gs_detail_preservation; /**< global smoothing detail preservation slider */
    QSlider * slider_fs_smoothness; /**< focalized smoothing smoothness slider */
    QSlider * slider_fs_radius; /**< focalized smoothing radius slider */

    QProgressBar * progress_bar; /**< global smoothing progress bar */

    QMenu * fileMenu; /**< file menu */
    QMenu * viewMenu; /**< view menu */
    QMenu * settingsMenu; /**< settings menu */
    QMenu * helpMenu; /**< help menu */

    ///Menu Actions
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

    myDataManager data; /**< Data manager for iunput and output meshes */

    QThread smoothingThread; /**< thread for global smoothing */
    GlobalSmoothingTask * smoothingTask; /**< global smoothing external step task */

    globalSmoothingAlgorithm currentGlobalSmoothingAlgorithm; /**< global smoothing algorithm flag */
    focalizedSmoothingAlgorithm currentFocalizedSmoothingAlgorithm; /**< focalized smoothing algorithm flag */

    bool selectionMode; /**< if user is selecting something in output mesh visualizer */
    bool runningStatus; /**< if global smoothing is running */

    bool globalSmoothingStopped;  /**< if global smoothing is stopped */

public:
    /**
     * @brief myMainWindow - default constructor: dynamic building of all the interface
     */
	myMainWindow();
    /**
     * @brief ~myMainWindow - destructor
     */
	~myMainWindow();
protected:
    /**
     * @brief keyPressEvent - Qt key press event
     * @param e event
     */
	void keyPressEvent(QKeyEvent* e);
    /**
     * @brief keyReleaseEvent - Qt key release event
     * @param e event
     */
	void keyReleaseEvent(QKeyEvent* e);
    /**
     * @brief eventFilter - Qt event filter
     * @param object current object
     * @param event current event
     * @return success
     */
	bool eventFilter(QObject *object, QEvent *event);
private:
    /**
     * @brief createActions - create menu actions and connect all signals and slots
     */
    void createActions();
    /**
     * @brief createMenus - create menus
     */
    void createMenus();
    /**
     * @brief updateWidgetValues - set default slider widget values (min, max and current)
     */
    void updateWidgetValues();
    /**
     * @brief setSmoothingThread - set thread for global smoothing
     */
    void setSmoothingThread();
    /**
     * @brief loadMesh - load input mesh from a file supported by OpenMesh library
     */
	void loadMesh();
    /**
     * @brief saveMesh - save output mesh to a file supported by OpenMesh library
     */
	void saveMesh();
    /**
     * @brief exit - close the application
     */
	void exit();
    /**
     * @brief flatMode - set triangle rendering
     */
    void flatMode();
    /**
     * @brief wireframeMode - set wireframe rendering
     */
	void wireframeMode();
    /**
     * @brief pointsMode - set point rendering
     */
	void pointsMode();
    /**
     * @brief setShaders - set shaders profile (customizable)
     */
	void setShaders();
    /**
     * @brief setGlobalSmoothingAlgorithm - set global smoothing algorithm
     */
	void setGlobalSmoothingAlgorithm();
    /**
     * @brief setFocalizedSmoothingAlgorithm - set focalized smoothing algorithm
     */
	void setFocalizedSmoothingAlgorithm();
    /**
     * @brief about - show about window
     */
	void about();
    /**
     * @brief userGuide - show user guide
     */
	void userGuide();
    /**
     * @brief enableSmoothingType - enable widgets for current smoothing type
     */
    void enableSmoothingType();
    /**
     * @brief runGlobalSmoothing - run global smoothing algorithm
     */
	void runGlobalSmoothing();
    /**
     * @brief stopGlobalSmoothing - stop global smoothing
     */
    void stopGlobalSmoothing();
    /**
     * @brief continueGlobalSmoothing - continue global smoothing
     */
    void continueGlobalSmoothing();
    /**
     * @brief updateGlobalSmoothing - global smoothing management (step by step)
     */
    void updateGlobalSmoothing();
    /**
     * @brief setGlobalSmoothingStatus - set current global smoothing status
     * @param current_status - global smoothing status
     */
    void setGlobalSmoothingStatus(globalSmoothingStatus current_status);
    /**
     * @brief reinitializeOutput - reinitialize output mesh (regarding input mesh)
     */
	void reinitializeOutput();
    /**
     * @brief setOutputAsInput - set output mesh as new input mesh
     */
	void setOutputAsInput();
    /**
     * @brief getSelection - get selected vertices for the current mouse position in the output mesh visualizer
     * @param selected_vertices_ids - output vector of selected vertices IDs
     */
	void getSelection(vector<size_t> & selected_vertices_ids);
    /**
     * @brief updateSelection - update selection in data manager and visualization
     */
	void updateSelection();
    /**
     * @brief removeSelection - remove selection from data manager and visualization
     */
	void removeSelection();
    /**
     * @brief selectAndSmooth - select and perform focalized smoothing regarding control widget values
     */
	void selectAndSmooth();
};


#endif // MYMAINWINDOW_H
