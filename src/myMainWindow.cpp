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

	push_button_reinitialize_data = new QPushButton("&Restart", this);
	push_button_update_input_data = new QPushButton("&Update", this);

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

	layout_data_manipulation = new QHBoxLayout;
	layout_data_manipulation->addWidget(push_button_reinitialize_data);
	layout_data_manipulation->addWidget(push_button_update_input_data);

	group_box_smoothing_type = new QGroupBox(tr("Smoothing type"));
	group_box_smoothing_type->setMaximumWidth(control_block_max_width);
	group_box_smoothing_type->setLayout(layout_smoothing_type);

	group_box_global_smoothing = new QGroupBox(tr("Global smoothing"));
	group_box_global_smoothing->setMaximumWidth(control_block_max_width);
	group_box_global_smoothing->setLayout(layout_global_smoothing);

	group_box_focalized_smoothing = new QGroupBox(tr("Focalized smoothing"));
	group_box_focalized_smoothing->setMaximumWidth(control_block_max_width);
	group_box_focalized_smoothing->setLayout(layout_focalized_smoothing);

	group_box_data_manipulation = new QGroupBox(tr("Data manipulation"));
	group_box_data_manipulation->setMaximumWidth(control_block_max_width);
	group_box_data_manipulation->setLayout(layout_data_manipulation);

	control_layout = new QVBoxLayout;
	control_layout->addWidget(group_box_smoothing_type);
	control_layout->addWidget(group_box_global_smoothing);
	control_layout->addWidget(group_box_focalized_smoothing);
	control_layout->addWidget(group_box_data_manipulation);
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

	widget = new QWidget;
	widget->setLayout(layout);
	setCentralWidget(widget);

	createActions();
	createMenus();
	setSmoothingThread();

	QString message = tr("A context menu is available by right-clicking");

	statusBar()->showMessage(message);

	setWindowTitle(tr("Mesh Smoothing Tool"));
	setMinimumSize(480, 320);
	resize(1200, 600);

	enableSmoothingType();

	//setMouseTracking(true);
	output_mesh_visualizer_ptr->installEventFilter(this);

	selectionMode = 0;
	runningStatus = 0;

	globalSmoothingStopped = 0;

	setGlobalSmoothingStatus(gs_status_init);

	currentGlobalSmoothingAlgorithm = gs_algorithm_bilateral_normal;
	currentFocalizedSmoothingAlgorithm = fs_algorithm_uniform_laplacian;
}

myMainWindow::~myMainWindow()
{
	smoothingThread.quit();
	smoothingThread.wait();
}

void myMainWindow::createActions()
{
	connect(push_button_global_smoothing_run, &QPushButton::released, this, &myMainWindow::runGlobalSmoothing);
	connect(push_button_global_smoothing_stop, &QPushButton::released, this, &myMainWindow::stopGlobalSmoothing);
	connect(push_button_global_smoothing_continue, &QPushButton::released, this, &myMainWindow::continueGlobalSmoothing);
	connect(radio_button_smoothing_type_g, &QRadioButton::released, this, &myMainWindow::enableSmoothingType);
	connect(radio_button_smoothing_type_f, &QRadioButton::released, this, &myMainWindow::enableSmoothingType);
	connect(push_button_reinitialize_data, &QPushButton::released, this, &myMainWindow::reinitializeOutput);
	connect(push_button_update_input_data, &QPushButton::released, this, &myMainWindow::setOutputAsInput);

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

	setGlobalSmoothingAlgorithmAct = new QAction(tr("&Set global smoothing algorithm"), this);
	setGlobalSmoothingAlgorithmAct->setStatusTip(tr("Set global smoothing algorithm"));
	connect(setGlobalSmoothingAlgorithmAct, &QAction::triggered, this, &myMainWindow::setGlobalSmoothingAlgorithm);

	setFocalizedSmoothingAlgorithmAct = new QAction(tr("&Set focalized smoothing algorithm"), this);
	setFocalizedSmoothingAlgorithmAct->setStatusTip(tr("Set focalized smoothing algorithm"));
	connect(setFocalizedSmoothingAlgorithmAct, &QAction::triggered, this, &myMainWindow::setFocalizedSmoothingAlgorithm);

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
	settingsMenu->addAction(setGlobalSmoothingAlgorithmAct);
	settingsMenu->addAction(setFocalizedSmoothingAlgorithmAct);

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(userGuideAct);
}

void myMainWindow::updateWidgetValues()
{
	///// initialize all widget values

	progress_bar->setMaximum(100);
	progress_bar->setMinimum(0);
	progress_bar->setValue(0);

	slider_gs_smoothness->setMaximum(100);
	slider_gs_smoothness->setMinimum(0);
	slider_gs_smoothness->setValue(5);

	slider_gs_radius_ratio->setMaximum(100);
	slider_gs_radius_ratio->setMinimum(0);
	slider_gs_radius_ratio->setValue(10);

	slider_gs_detail_preservation->setMaximum(100);
	slider_gs_detail_preservation->setMinimum(0);
	slider_gs_detail_preservation->setValue(50);

	slider_fs_smoothness->setMaximum(50);
	slider_fs_smoothness->setMinimum(0);
	slider_fs_smoothness->setValue(3);

	slider_fs_radius->setMaximum(20);
	slider_fs_radius->setMinimum(0);
	slider_fs_radius->setValue(3);
}

void myMainWindow::loadMesh()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Load Mesh"), "",
		tr("mesh file format (*.obj *.off *.ply *.stl);;All Files (*)"));
	if (fileName == "") return;
	statusBar()->showMessage("Loading ... "+fileName);
	data.loadInputMesh(fileName.toStdString());
	data.updateShapes();
	input_mesh_visualizer_ptr->setShape(&data.input_mesh_shape);
	output_mesh_visualizer_ptr->setShape(&data.output_mesh_shape);
	statusBar()->showMessage("Done");
	updateWidgetValues();
}

void myMainWindow::saveMesh()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Mesh"), "",
		tr("mesh file format (*.obj *.off *.ply *.stl);;All Files (*)"));
	if (fileName == "") return;
	statusBar()->showMessage("Saving ... " + fileName);
	data.saveOutputMesh(fileName.toStdString());
	statusBar()->showMessage("Done");
}

void myMainWindow::exit()
{
	close();
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
	QInputDialog qDialog;

	QStringList items;
	items << QString("Flat");
	items << QString("Phong");
	items << QString("Normal Map");

	qDialog.setOptions(QInputDialog::UseListViewForComboBoxItems);
	qDialog.setComboBoxItems(items);
	qDialog.setWindowTitle("Shaders");
	qDialog.setLabelText("Choose shader options: ");

	if (!qDialog.exec()) return;

	if (qDialog.textValue().toStdString() == "Flat")
	{
		input_mesh_visualizer_ptr->clearAndDeleteShaders();
		input_mesh_visualizer_ptr->addShader(GL_VERTEX_SHADER, "VertexShaderCodeFlat.glsl");
		input_mesh_visualizer_ptr->addShader(GL_FRAGMENT_SHADER, "FragmentShaderCodeFlat.glsl");
		input_mesh_visualizer_ptr->installShaders();

		output_mesh_visualizer_ptr->clearAndDeleteShaders();
		output_mesh_visualizer_ptr->addShader(GL_VERTEX_SHADER, "VertexShaderCodeFlat.glsl");
		output_mesh_visualizer_ptr->addShader(GL_FRAGMENT_SHADER, "FragmentShaderCodeFlat.glsl");
		output_mesh_visualizer_ptr->installShaders();

		QString message = tr(qDialog.textValue().toStdString().c_str());
		statusBar()->showMessage(message);
	}
	else if (qDialog.textValue().toStdString() == "Phong")
	{
		input_mesh_visualizer_ptr->clearAndDeleteShaders();
		input_mesh_visualizer_ptr->addShader(GL_VERTEX_SHADER, "VertexShaderCodePhong.glsl");
		input_mesh_visualizer_ptr->addShader(GL_FRAGMENT_SHADER, "FragmentShaderCodePhong.glsl");
		input_mesh_visualizer_ptr->installShaders();

		output_mesh_visualizer_ptr->clearAndDeleteShaders();
		output_mesh_visualizer_ptr->addShader(GL_VERTEX_SHADER, "VertexShaderCodePhong.glsl");
		output_mesh_visualizer_ptr->addShader(GL_FRAGMENT_SHADER, "FragmentShaderCodePhong.glsl");
		output_mesh_visualizer_ptr->installShaders();

		QString message = tr(qDialog.textValue().toStdString().c_str());
		statusBar()->showMessage(message);
	}
	else if (qDialog.textValue().toStdString() == "Normal Map")
	{
		input_mesh_visualizer_ptr->clearAndDeleteShaders();
		input_mesh_visualizer_ptr->addShader(GL_VERTEX_SHADER, "VertexShaderCodeNormalMap.glsl");
		input_mesh_visualizer_ptr->addShader(GL_FRAGMENT_SHADER, "FragmentShaderCodeNormalMap.glsl");
		input_mesh_visualizer_ptr->installShaders();

		output_mesh_visualizer_ptr->clearAndDeleteShaders();
		output_mesh_visualizer_ptr->addShader(GL_VERTEX_SHADER, "VertexShaderCodeNormalMap.glsl");
		output_mesh_visualizer_ptr->addShader(GL_FRAGMENT_SHADER, "FragmentShaderCodeNormalMap.glsl");
		output_mesh_visualizer_ptr->installShaders();

		QString message = tr(qDialog.textValue().toStdString().c_str());
		statusBar()->showMessage(message);
	}
}

void myMainWindow::setGlobalSmoothingAlgorithm()
{
	QInputDialog qDialog;

	QStringList items;
	items << QString(globalSmoothingAlgorithmLabels[gs_algorithm_bilateral_normal].c_str());
	items << QString(globalSmoothingAlgorithmLabels[gs_algorithm_guided].c_str());

	qDialog.setOptions(QInputDialog::UseListViewForComboBoxItems);
	qDialog.setComboBoxItems(items);
	qDialog.setWindowTitle("Global Smoothing Algorithm");
	qDialog.setLabelText("Choose global smoothing algorithm: ");

	qDialog.setTextValue(globalSmoothingAlgorithmLabels[currentGlobalSmoothingAlgorithm].c_str());

	if (qDialog.exec())
	{
		if (qDialog.textValue().toStdString() == globalSmoothingAlgorithmLabels[gs_algorithm_bilateral_normal])
			currentGlobalSmoothingAlgorithm = gs_algorithm_bilateral_normal;
		else if (qDialog.textValue().toStdString() == globalSmoothingAlgorithmLabels[gs_algorithm_guided])
			currentGlobalSmoothingAlgorithm = gs_algorithm_guided;
		else
			currentGlobalSmoothingAlgorithm = gs_algorithm_bilateral_normal;
		QString message = tr(globalSmoothingAlgorithmLabels[currentGlobalSmoothingAlgorithm].c_str());
		statusBar()->showMessage(message);
	}
}

void myMainWindow::setFocalizedSmoothingAlgorithm()
{
	QInputDialog qDialog;

	QStringList items;
	items << QString(focalizedSmoothingAlgorithmLabels[fs_algorithm_uniform_laplacian].c_str());
	items << QString(focalizedSmoothingAlgorithmLabels[fs_algorithm_hc_laplacian].c_str());

	qDialog.setOptions(QInputDialog::UseListViewForComboBoxItems);
	qDialog.setComboBoxItems(items);
	qDialog.setWindowTitle("Focalized Smoothing Algorithm");
	qDialog.setLabelText("Choose focalized smoothing algorithm: ");

	qDialog.setTextValue(focalizedSmoothingAlgorithmLabels[currentFocalizedSmoothingAlgorithm].c_str());

	if (qDialog.exec())
	{
		if (qDialog.textValue().toStdString() == focalizedSmoothingAlgorithmLabels[fs_algorithm_uniform_laplacian])
			currentFocalizedSmoothingAlgorithm = fs_algorithm_uniform_laplacian;
		else if (qDialog.textValue().toStdString() == focalizedSmoothingAlgorithmLabels[fs_algorithm_hc_laplacian])
			currentFocalizedSmoothingAlgorithm = fs_algorithm_hc_laplacian;
		else
			currentFocalizedSmoothingAlgorithm = fs_algorithm_uniform_laplacian;
		QString message = tr(focalizedSmoothingAlgorithmLabels[currentFocalizedSmoothingAlgorithm].c_str());
		statusBar()->showMessage(message);
	}
}

void myMainWindow::about()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("About");
	msgBox.setText("Mesh Smoothing Tool v1.0");
	msgBox.setInformativeText("A mesh processing tool for surface smoothing");
	int ret = msgBox.exec();
}

void myMainWindow::userGuide()
{
	QString link = "http://www.google.com";
	QDesktopServices::openUrl(QUrl(link));
}

void myMainWindow::setGlobalSmoothingStatus( globalSmoothingStatus current_status)
{
	if (current_status == gs_status_init)
	{
		push_button_global_smoothing_run->setEnabled(true);
		push_button_global_smoothing_stop->setEnabled(false);
		push_button_global_smoothing_continue->setEnabled(false);
		group_box_smoothing_type->setEnabled(true);
		group_box_data_manipulation->setEnabled(true);
		menuBar()->setEnabled(true);
		runningStatus = 0;
		globalSmoothingStopped = 0;
		QString message = tr("Ready ... ");
		statusBar()->showMessage(message);
	}
	else if (current_status == gs_status_started)
	{
		runningStatus = 1;
		globalSmoothingStopped = 0;
		progress_bar->setValue(0);
		push_button_global_smoothing_run->setEnabled(false);
		push_button_global_smoothing_stop->setEnabled(true);
		push_button_global_smoothing_continue->setEnabled(false);
		group_box_smoothing_type->setEnabled(false);
		group_box_data_manipulation->setEnabled(false);
		menuBar()->setEnabled(false);
		QString message = tr("Running ... ");
		statusBar()->showMessage(message);
	}
	else if (current_status == gs_status_stopping)
	{
		globalSmoothingStopped = 1;
		push_button_global_smoothing_stop->setEnabled(false);
		push_button_global_smoothing_continue->setEnabled(false);
		QString message = tr("Stopping ... ");
		statusBar()->showMessage(message);
	}
	else if (current_status == gs_status_stopped)
	{
		push_button_global_smoothing_run->setEnabled(true);
		group_box_smoothing_type->setEnabled(true);
		group_box_data_manipulation->setEnabled(true);
		menuBar()->setEnabled(true);
		push_button_global_smoothing_stop->setEnabled(false);
		push_button_global_smoothing_continue->setEnabled(true);
		runningStatus = 0;
		globalSmoothingStopped = 0;
		QString message = tr("Stopped ... ");
		statusBar()->showMessage(message);
	}
	else if (current_status == gs_status_continuing)
	{
		runningStatus = 1;
		globalSmoothingStopped = 0;
		push_button_global_smoothing_run->setEnabled(false);
		push_button_global_smoothing_stop->setEnabled(true);
		push_button_global_smoothing_continue->setEnabled(false);
		group_box_smoothing_type->setEnabled(false);
		group_box_data_manipulation->setEnabled(false);
		menuBar()->setEnabled(false);
		QString message = tr("Continuing ... ");
		statusBar()->showMessage(message);
	}
}

void myMainWindow::setSmoothingThread()
{
	smoothingTask = new GlobalSmoothingTask;
	smoothingTask->moveToThread(&smoothingThread);
	smoothingTask->current_thread = &smoothingThread;
	connect(&smoothingThread, &QThread::started, smoothingTask, &GlobalSmoothingTask::run);
	connect(&smoothingThread, &QThread::finished, this, &myMainWindow::updateGlobalSmoothing);
	smoothingTask->data = &data;
}

void myMainWindow::stopGlobalSmoothing()
{
	setGlobalSmoothingStatus(gs_status_stopping);
}

void myMainWindow::continueGlobalSmoothing()
{
	smoothingThread.start();
	setGlobalSmoothingStatus(gs_status_continuing);
}

void myMainWindow::runGlobalSmoothing()
{
	const int default_iteration_step_size = 1;
	int smoothness_i = slider_gs_smoothness->value();
	float smoothness = static_cast<float>(slider_gs_smoothness->value());
	float radius_ratio = static_cast<float>(slider_gs_radius_ratio->value());
	float detail_preservation = static_cast<float>(slider_gs_detail_preservation->value());
	smoothingTask->n_vertex_iterations = static_cast<int>(smoothness/30.0f + 7.0f);
	smoothingTask->sigma_c_ratio = 0.5 + radius_ratio / 20.0f + smoothness/100.0f;
	smoothingTask->sigma_s = 0.8f-detail_preservation * 0.006f;
	cout << smoothingTask->sigma_s << endl;
	smoothingTask->iteration_step_size = default_iteration_step_size;
	smoothingTask->currentGlobalSmoothingIteration = 0;
	smoothingTask->finalGlobalSmoothingIteration = smoothness_i / default_iteration_step_size;
	smoothingTask->algorithm_flag = currentGlobalSmoothingAlgorithm;
	smoothingThread.start();
	setGlobalSmoothingStatus(gs_status_started);
}

void myMainWindow::updateGlobalSmoothing()
{
	int current_iteration = smoothingTask->currentGlobalSmoothingIteration;
	int final_iteration = smoothingTask->finalGlobalSmoothingIteration;
	if (globalSmoothingStopped)
	{
		if (current_iteration > 0)
		{
			smoothingTask->currentGlobalSmoothingIteration--;
			current_iteration--;
		}
		setGlobalSmoothingStatus(gs_status_stopped);
	}
	else if (current_iteration >= final_iteration)
	{
		smoothingTask->updateData();
		data.updateShapes();
		setGlobalSmoothingStatus(gs_status_init);
	}
	else
	{
		smoothingTask->updateData();
		data.updateShapes();
		smoothingTask->currentGlobalSmoothingIteration ++;
		smoothingThread.start();
	}
	output_mesh_visualizer_ptr->updateMesh();
	output_mesh_visualizer_ptr->repaint();
	progress_bar->setValue(static_cast<int>((static_cast<float>(current_iteration) / static_cast<float>(final_iteration))*100.0f));
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

void myMainWindow::reinitializeOutput()
{
	data.reinitialize();
	input_mesh_visualizer_ptr->updateMesh();
	input_mesh_visualizer_ptr->repaint();
	output_mesh_visualizer_ptr->updateMesh();
	output_mesh_visualizer_ptr->repaint();
}

void myMainWindow::setOutputAsInput()
{
	data.setOutputAsInput();
	input_mesh_visualizer_ptr->updateMesh();
	input_mesh_visualizer_ptr->repaint();
	output_mesh_visualizer_ptr->updateMesh();
	output_mesh_visualizer_ptr->repaint();
}

void myMainWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_Shift:
		if (selectionMode == 0)
		{
			selectionMode = 1;
			updateSelection();
			output_mesh_visualizer_ptr->repaint();
		}
		break;
	}
}

void myMainWindow::keyReleaseEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_Shift:
		if (selectionMode == 1)
		{
			selectionMode = 0;
			removeSelection();
		}
		break;
	}
}

bool myMainWindow::eventFilter(QObject *object, QEvent *event)
{
	if (object == output_mesh_visualizer_ptr)
	{
		if (event->type() == QEvent::MouseMove)
		{
			QMouseEvent * e = static_cast<QMouseEvent *>(event);
			if (e->modifiers() & Qt::ShiftModifier)
			{
				selectionMode = 1;
				updateSelection();
				return true;
			}
			else
			{
				selectionMode = 0;
				removeSelection();
				return false;
			}
		}
		if (event->type() == QEvent::MouseButtonRelease)
		{
			QMouseEvent * e = static_cast<QMouseEvent *>(event);
			if (e->modifiers() & Qt::ShiftModifier)
			{
				if (e->button() == Qt::LeftButton)
				{
					cout << "Focalized Smoothing ... ";
					selectionMode = 1;
					selectAndSmooth();
					return true;
				}
			}
		}
	}
	return QWidget::eventFilter(object, event);
}

void myMainWindow::getSelection(vector<size_t> & selected_vertices_ids)
{
	selected_vertices_ids.clear();
	int k = slider_fs_radius->value();
	glm::vec2 mousePos = output_mesh_visualizer_ptr->getCurrentMousePosition();
	glm::vec3 dir = output_mesh_visualizer_ptr->getRayDirection(mousePos);
	//cout << dir.x << " " << dir.y << " " << dir.z << endl;
	vector<TriMesh::VertexHandle> selected_vertices;
	glm::mat4 trMatrix = output_mesh_visualizer_ptr->getModelToWorldMatrix();
	//compute neighborhood
	float min_dist = 999999.0f;
	TriMesh::VertexHandle nearest_vertex_handle;
	for (TriMesh::FaceIter f_it = data.output_mesh.faces_begin(); f_it != data.output_mesh.faces_end(); f_it++)
	{
		glm::vec3 a, b, c;
		TriMesh::VertexHandle a_vh, b_vh, c_vh;

		TriMesh::FaceVertexIter fv_it = data.output_mesh.fv_iter(*f_it);
		a = glm::vec3(data.output_mesh.point(*fv_it)[0], data.output_mesh.point(*fv_it)[1], data.output_mesh.point(*fv_it)[2]);
		a = glm::vec3(trMatrix*glm::vec4(a, 1.0));
		a_vh = *fv_it;
		fv_it++;
		b = glm::vec3(data.output_mesh.point(*fv_it)[0], data.output_mesh.point(*fv_it)[1], data.output_mesh.point(*fv_it)[2]);
		b = glm::vec3(trMatrix*glm::vec4(b, 1.0));
		b_vh = *fv_it;
		fv_it++;
		c = glm::vec3(data.output_mesh.point(*fv_it)[0], data.output_mesh.point(*fv_it)[1], data.output_mesh.point(*fv_it)[2]);
		c = glm::vec3(trMatrix*glm::vec4(c, 1.0));
		c_vh = *fv_it;

		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;

		glm::vec3 normal = glm::cross(ab, ac);

		glm::vec3 o = output_mesh_visualizer_ptr->getCamera()->getPosition();
		float val = glm::dot(dir, normal);
		if (val == 0.0f) continue;
		float ti = glm::dot((a - o), normal) / val;
		glm::vec3 temp = dir*ti;
		glm::vec3 pi = o + temp;
		glm::vec3 v0 = b - a;
		glm::vec3 v1 = c - a;
		glm::vec3 v2 = pi - a;
		float d00 = glm::dot(v0, v0);
		float d01 = glm::dot(v0, v1);
		float d11 = glm::dot(v1, v1);
		float d20 = glm::dot(v2, v0);
		float d21 = glm::dot(v2, v1);
		double denom = glm::dot(d00, d11) - glm::dot(d01, d01);
		float v = (d11 * d20 - d01 * d21) / denom;
		float w = (d00 * d21 - d01 * d20) / denom;
		float u = 1.0 - v - w;

		bool liesOutside = v>1 || v<0 || w>1 || w<0 || u>1 || u<0;

		if (!liesOutside)
		{
			//cout << "intersect" << endl;
			if (ti < min_dist  && ti >= 0)
			{
				if (u>v)
				{
					if (u > w)
						nearest_vertex_handle = a_vh;
					else
						nearest_vertex_handle = c_vh;
				}
				else if (v > w)
					nearest_vertex_handle = b_vh;
				else
					nearest_vertex_handle = c_vh;
				min_dist = ti;
			}

		}
	}
	if (nearest_vertex_handle.is_valid())
	{
		getVertexNeighbors(data.output_mesh, nearest_vertex_handle, k, selected_vertices);
		selected_vertices.push_back(nearest_vertex_handle);
	}

	for (size_t i = 0; i < selected_vertices.size(); i++)
		selected_vertices_ids.push_back(selected_vertices[i].idx());
}

void myMainWindow::updateSelection()
{
	if (!runningStatus && radio_button_smoothing_type_f->isChecked())
	{
		vector<size_t> selected_vertices_ids;
		getSelection(selected_vertices_ids);
		data.updateOutputSelection(selected_vertices_ids);
		output_mesh_visualizer_ptr->setSelection(&data.selection);
		output_mesh_visualizer_ptr->repaint();
	}
}

void myMainWindow::selectAndSmooth()
{
	if (!runningStatus && radio_button_smoothing_type_f->isChecked())
	{
		vector<size_t> selected_vertices_ids;
		getSelection(selected_vertices_ids);
		int num_its = slider_fs_smoothness->value();
		if (currentFocalizedSmoothingAlgorithm == fs_algorithm_uniform_laplacian)
			data.output_mesh = uniformLaplacian(data.output_mesh, num_its, 1.0f, selected_vertices_ids);
		else if ((currentFocalizedSmoothingAlgorithm == fs_algorithm_hc_laplacian))
			data.output_mesh = HCLaplacian(data.output_mesh, num_its, 0.5f,0.5f,selected_vertices_ids);
		else
			data.output_mesh = uniformLaplacian(data.output_mesh, num_its, 1.0f, selected_vertices_ids);
		data.updateOutputShape();
		data.updateOutputSelection(selected_vertices_ids);
		output_mesh_visualizer_ptr->setSelection(&data.selection);
		output_mesh_visualizer_ptr->repaint();
	}
}

void myMainWindow::removeSelection()
{
	output_mesh_visualizer_ptr->removeSelection();
	output_mesh_visualizer_ptr->repaint();
}
