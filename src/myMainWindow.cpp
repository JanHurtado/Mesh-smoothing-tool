#include "myMainWindow.h"

myMainWindow::myMainWindow()
{
	const int control_block_max_width = 250;

	input_mesh_visualizer_ptr = new myGLWindow(this);
	output_mesh_visualizer_ptr = new myGLWindow(this);

	radio_button_smoothing_type_g = new QRadioButton("Global");
	radio_button_smoothing_type_f = new QRadioButton("Focalized");
	radio_button_smoothing_type_g->setChecked(true);

	push_button_global_smoothing_run = new QPushButton("&Run", this);
	push_button_global_smoothing_stop = new QPushButton("&Stop", this);
	push_button_global_smoothing_continue = new QPushButton("&Continue", this);

	slider_gs_smoothness = new QSlider(Qt::Horizontal, this);
	slider_gs_radius_ratio = new QSlider(Qt::Horizontal, this);
	slider_gs_detail_preservation = new QSlider(Qt::Horizontal, this);

	progress_bar = new QProgressBar(this);

	slider_fs_smoothness = new QSlider(Qt::Horizontal, this);
	slider_fs_radius = new QSlider(Qt::Horizontal, this);

	layout_smoothing_type = new QHBoxLayout;
	layout_smoothing_type->addWidget(radio_button_smoothing_type_g);
	layout_smoothing_type->addWidget(radio_button_smoothing_type_f);

	layout_global_smoothing_buttons = new QHBoxLayout;
	layout_global_smoothing_buttons->addWidget(push_button_global_smoothing_run);
	layout_global_smoothing_buttons->addWidget(push_button_global_smoothing_stop);
	layout_global_smoothing_buttons->addWidget(push_button_global_smoothing_continue);

	layout_global_smoothing = new QVBoxLayout;
	layout_global_smoothing->addWidget(new QLabel("Smoothness", this));
	layout_global_smoothing->addWidget(slider_gs_smoothness);
	layout_global_smoothing->addWidget(new QLabel("Influence radius ratio",this));
	layout_global_smoothing->addWidget(slider_gs_radius_ratio);
	layout_global_smoothing->addWidget(new QLabel("Detail preservation", this));
	layout_global_smoothing->addWidget(slider_gs_detail_preservation);
	layout_global_smoothing->addWidget(progress_bar);
	layout_global_smoothing->addLayout(layout_global_smoothing_buttons);
	
	layout_focalized_smoothing = new QVBoxLayout;
	layout_focalized_smoothing->addWidget(new QLabel("Smoothness", this));
	layout_focalized_smoothing->addWidget(slider_fs_smoothness);
	layout_focalized_smoothing->addWidget(new QLabel("Radius", this));
	layout_focalized_smoothing->addWidget(slider_fs_radius);


	group_box_smoothing_type = new QGroupBox(tr("Smoothing type"));
	group_box_smoothing_type->setMaximumWidth(control_block_max_width);
	group_box_smoothing_type->setLayout(layout_smoothing_type);

	group_box_global_smoothing = new QGroupBox(tr("Global smoothing"));
	group_box_global_smoothing->setMaximumWidth(control_block_max_width);
	group_box_global_smoothing->setLayout(layout_global_smoothing);

	group_box_focalized_smoothing = new QGroupBox(tr("Focalized smoothing"));
	group_box_focalized_smoothing->setMaximumWidth(control_block_max_width);
	group_box_focalized_smoothing->setLayout(layout_focalized_smoothing);

	control_layout = new QVBoxLayout;
	control_layout->addWidget(group_box_smoothing_type);
	control_layout->addWidget(group_box_global_smoothing);
	control_layout->addWidget(group_box_focalized_smoothing);
	control_layout->addStretch();

	QWidget * t_widget = new QWidget(this);
	t_widget->setLayout(control_layout);

	control_scroll_area = new QScrollArea(this);
	control_scroll_area->setWidget(t_widget);
	control_scroll_area->setMaximumWidth(control_block_max_width+40);

	layout = new QHBoxLayout;
	layout->addWidget(input_mesh_visualizer_ptr);
	layout->addWidget(output_mesh_visualizer_ptr);
	layout->addWidget(control_scroll_area);
	//layout->addLayout(control_layout);

	widget = new QWidget;
	widget->setLayout(layout);
	setCentralWidget(widget);

	createActions();
	createMenus();

	QString message = tr("A context menu is available by right-clicking");

	statusBar()->showMessage(message);

	setWindowTitle(tr("Mesh Smoothing Tool"));
	setMinimumSize(480, 320);
	resize(1200, 600);

	enableSmoothingType();
}

myMainWindow::~myMainWindow()
{
}

void myMainWindow::createActions()
{
	connect(push_button_global_smoothing_run, &QPushButton::released, this, &myMainWindow::runSmoothingGlobal);
	connect(radio_button_smoothing_type_g, &QRadioButton::released, this, &myMainWindow::enableSmoothingType);
	connect(radio_button_smoothing_type_f, &QRadioButton::released, this, &myMainWindow::enableSmoothingType);


	loadAct = new QAction(tr("&Load"), this);
	loadAct->setShortcuts(QKeySequence::Open);
	loadAct->setStatusTip(tr("Load input mesh"));
	connect(loadAct, &QAction::triggered, this, &myMainWindow::loadMesh);

	saveAct = new QAction(tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save output mesh"));
	connect(saveAct, &QAction::triggered, this, &myMainWindow::saveMesh);

	exitAct = new QAction(tr("&Exit"), this);
	exitAct->setShortcuts(QKeySequence::Close);
	exitAct->setStatusTip(tr("Exit program"));
	connect(exitAct, &QAction::triggered, this, &myMainWindow::exit);

	flatModeAct = new QAction(tr("&Flat"), this);
	flatModeAct->setStatusTip(tr("Flat rendering"));
	connect(flatModeAct, &QAction::triggered, this, &myMainWindow::flatMode);

	wireframeModeAct = new QAction(tr("&Wireframe"), this);
	wireframeModeAct->setStatusTip(tr("Wireframe rendering"));
	connect(wireframeModeAct, &QAction::triggered, this, &myMainWindow::wireframeMode);

	pointsModeAct = new QAction(tr("&Points"), this);
	pointsModeAct->setStatusTip(tr("Point rendering"));
	connect(pointsModeAct, &QAction::triggered, this, &myMainWindow::pointsMode);

	setShadersAct = new QAction(tr("&Set shaders"), this);
	setShadersAct->setStatusTip(tr("Set shaders"));
	connect(setShadersAct, &QAction::triggered, this, &myMainWindow::setShaders);

	setAlgorithmsAct = new QAction(tr("&Set algorithms"), this);
	setAlgorithmsAct->setStatusTip(tr("Set algorithms"));
	connect(setAlgorithmsAct, &QAction::triggered, this, &myMainWindow::setAlgorithms);

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("About"));
	connect(aboutAct, &QAction::triggered, this, &myMainWindow::about);

	userGuideAct = new QAction(tr("&User Guide"), this);
	userGuideAct->setStatusTip(tr("User Guide"));
	connect(userGuideAct, &QAction::triggered, this, &myMainWindow::userGuide);
}

void myMainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(loadAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(exitAct);

	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(flatModeAct);
	viewMenu->addAction(wireframeModeAct);
	viewMenu->addAction(pointsModeAct);

	settingsMenu = menuBar()->addMenu(tr("&Settings"));
	settingsMenu->addAction(setShadersAct);
	settingsMenu->addAction(setAlgorithmsAct);

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(userGuideAct);
}

void myMainWindow::loadMesh()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Load Mesh"), "",
		tr("mesh file format (*.obj *.off *.ply *.stl);;All Files (*)"));
	statusBar()->showMessage("Loading ... "+fileName);
	data.loadInputMesh(fileName.toStdString());
	data.updateShapes();
	input_mesh_visualizer_ptr->setShape(&data.input_mesh_shape);
	output_mesh_visualizer_ptr->setShape(&data.output_mesh_shape);
	statusBar()->showMessage("Done");
}

void myMainWindow::saveMesh()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Mesh"), "",
		tr("mesh file format (*.obj *.off *.ply *.stl);;All Files (*)"));
	statusBar()->showMessage("Saving ... " + fileName);
	data.saveOutputMesh(fileName.toStdString());
	statusBar()->showMessage("Done");
}

void myMainWindow::exit()
{

}

void myMainWindow::flatMode()
{
	input_mesh_visualizer_ptr->setVisualizationMode(e_draw_faces);
	output_mesh_visualizer_ptr->setVisualizationMode(e_draw_faces);
}

void myMainWindow::wireframeMode()
{
	input_mesh_visualizer_ptr->setVisualizationMode(e_draw_wireframe);
	output_mesh_visualizer_ptr->setVisualizationMode(e_draw_wireframe);
}

void myMainWindow::pointsMode()
{
	input_mesh_visualizer_ptr->setVisualizationMode(e_draw_points);
	output_mesh_visualizer_ptr->setVisualizationMode(e_draw_points);
}

void myMainWindow::setShaders()
{

}

void myMainWindow::setAlgorithms()
{

}

void myMainWindow::about()
{

}

void myMainWindow::userGuide()
{

}

void myMainWindow::runSmoothingGlobal()
{
	cout << "Smoothing ..." << endl;
}

void myMainWindow::enableSmoothingType()
{
	if (radio_button_smoothing_type_g->isChecked())
	{
		group_box_global_smoothing->setEnabled(true);
		group_box_focalized_smoothing->setEnabled(false);
	}
	else
	{
		group_box_global_smoothing->setEnabled(false);
		group_box_focalized_smoothing->setEnabled(true);
	}
}