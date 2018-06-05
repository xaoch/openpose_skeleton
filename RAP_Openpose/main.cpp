// ------------------------- OpenPose Library Tutorial - Thread - Example 2 - User Processing Function -------------------------
// This fourth example shows the user how to:
// 1. Read folder of images / video / webcam  (`producer` module)
// 2. Use the processing implemented by the user
// 3. Display the rendered pose (`gui` module)
// Everything in a multi-thread scenario (`thread` module) 
// In addition to the previous OpenPose modules, we also need to use:
// 1. `core` module: for the Datum struct that the `thread` module sends between the queues
// 2. `utilities` module: for the error & logging functions, i.e. op::error & op::log respectively
#include <cstdlib>

// 3rdparty dependencies
#include <gflags/gflags.h> // DEFINE_bool, DEFINE_int32, DEFINE_int64, DEFINE_uint64, DEFINE_double, DEFINE_string
#include <glog/logging.h> // google::InitGoogleLogging
// OpenPose dependencies
#include <openpose/headers.hpp>
//#include <boost-1_61\boost\python.hpp>
#include <boost\python.hpp>
#include "python_funs.h"
#include "params.h"
#include "headpose.h"
#include "headpose_output.h"
#include "UserDatum.h"
#include "mqtt.h"	
#include "config.h"
#include <thread>
#include <mutex>
#include <grpc++/grpc++.h>
#include "FeatureExtractionApi.grpc.pb.h"
#include "audience.h"
#include "openpose_no_grpc.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::Status;

namespace python = boost::python;
#define TO_DEGREES 180/3.141592654

bool globals::record = false;

// Mutex para el predictor
std::mutex mutex_for_python;
std::string globals::filename = "results";

std::string result_directory = "results";
//mqtt::async_client *client;
mqtt::async_client *globals::client = new mqtt::async_client(SERVER_ADDRESS, CLIENT_ID);
// If the user needs his own variables, he can inherit the op::Datum struct and add them
// UserDatum can be directly used by the OpenPose wrapper because it inherits from op::Datum, just define Wrapper<UserDatum> instead of
// Wrapper<op::Datum>
struct UserDatum : public op::Datum
{
	bool boolThatUserNeedsForSomeReason;

	UserDatum(const bool boolThatUserNeedsForSomeReason_ = false) :
		boolThatUserNeedsForSomeReason{ boolThatUserNeedsForSomeReason_ }
	{}
};


// The W-classes can be implemented either as a template or as simple classes given
// that the user usually knows which kind of data he will move between the queues,
// in this case we assume a std::shared_ptr of a std::vector of UserDatum

// This worker will just read and return all the jpg files in a directory
class GRPC_Input : public op::WorkerProducer<std::shared_ptr<std::vector<op::Datum>>>
{

private:
	ServerReader<Image>* reader;

public:
	GRPC_Input(ServerReader<Image>* _reader) :reader{_reader} {}


	void initializationOnThread() {}

	std::shared_ptr<std::vector<op::Datum>> workProducer()
	{	
		try {
			auto datumsPtr = std::make_shared<std::vector<op::Datum>>();
			Image image;
			//Sleep(9000);
			reader->Read(&image);
			std::string source(image.source());
			std::vector<char> data(image.source().begin(), image.source().end());
			data.push_back('\0');	
			datumsPtr->emplace_back();
			auto& datum = datumsPtr->at(0);
			//clock_t t_ini = clock();
			datum.cvInputData = cv::imdecode(cv::Mat(data), 1);
			//datum.cvInputData = cv::imread("homer.jpg");
			//clock_t t_fin = clock();
			//double secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
			//printf(" Imagen leida %.16g segundos\n", secs);
			//cv::Mat i = cv::imdecode(cv::Mat(data), 1);	
			if (datum.cvInputData.empty())
			{
				op::log("Empty frame detected. Closing program.", op::Priority::High);
				this->stop();
				datumsPtr = nullptr;
				
			}
			return datumsPtr;
		}
		catch (const std::exception& e) {
			op::log("Some kind of unexpected error happened.");
			this->stop();
			op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
			return nullptr;
		}

	}

};

class FeatureExtractionImpl final : public FeatureExtraction::Service {
		
		cv::Size getSizeImage(Image image) {
			std::string source(image.source());
			std::vector<char> data(image.source().begin(), image.source().end());
			data.push_back('\0');
			cv::Mat matImage = cv::imdecode(cv::Mat(data), 1);
			return matImage.size();
		}
		
		//void sendImagesToServer(std::string main_folder, std::string studentId_folder) {
			
			
			/*try {
				python::object result = python_funs::sendImagesFunc(main_folder, studentId_folder);
			}
			catch(python::error_already_set&){
				std::cout << "ERROR PYTHON" << std::endl;
				python::handle_exception();

			}*/

			/*std::string command = "python sendDataToServer.py send_images " + result_directory + " " + studentId_folder;
			system(command.c_str());*/
			//std::cout << "**DONE python1 " << std::endl;
		//}

		//void sendCSVToServer(std::string main_folder, std::string studentId_folder, std::string csv_name, std::ofstream *resultFile) {
			//resultFile->close();
			/*
			try {
				python::object result = python_funs::sendCSVFunc(main_folder, studentId_folder, csv_name);
			}
			catch (python::error_already_set&) {
				std::cout << "ERROR PYTHON" << std::endl;
				python::handle_exception();

			}*/
			
			/*std::string command = "python sendDataToServer.py send_results " + main_folder + " " + studentId_folder+" "+csv_name;
			system(command.c_str());
			*/
			//std::cout << "**DONE python2 " << std::endl;
		//}
		
		void senDataToServer(std::string main_folder, std::string studentId_folder, std::string csv_name, std::ofstream *resultFile) {
			resultFile->close();
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
			ZeroMemory(&pi, sizeof(pi));
			
			std::string command = "python sendDataToServer.py " + main_folder + " " + studentId_folder + " " + csv_name;
			
			// Start the child process. 
			if (!CreateProcess(NULL,
				const_cast<char *>(command.c_str()),
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi)           // Pointer to PROCESS_INFORMATION structure
				)
			{
				printf("CreateProcess failed (%d).\n", GetLastError());
				return;
			}
		}
		
		void deleteFolders(std::string filename) {
			std::string path = result_directory +"/"+ filename;
			//filesystem::remove_all(path);
			std::string actualPath = path + "/center";
			filesystem::remove_all(actualPath);
			actualPath = path + "/up";
			filesystem::remove_all(actualPath);
			actualPath = path + "/down";
			filesystem::remove_all(actualPath);
			actualPath = path + "/right";
			filesystem::remove_all(actualPath);
			actualPath = path + "/left";
			filesystem::remove_all(actualPath);
			actualPath = path + "/tv";
			filesystem::remove_all(actualPath);
			actualPath = path + "/good";
			filesystem::remove_all(actualPath);
			actualPath = path + "/bad";
			filesystem::remove_all(actualPath);
		}

		std::vector<int> getRandomValues(int maxRandomValue,int maxSelectedValues) {
			
			std::vector<int> selectedValues;
			int randomValue = 0;
			std::vector<int>::iterator it;
			
			if (maxSelectedValues == 0)
				return selectedValues;
			
			if (maxRandomValue < maxSelectedValues) {
				selectedValues.resize(maxSelectedValues);
				for (int i = 0; i < maxRandomValue;i++) 
					selectedValues.at(i) = i + 1;
				//std::for_each(selectedValues.begin(), selectedValues.end(), myobject);
				return selectedValues;
			}
			srand(time(NULL));
			do {
				
				randomValue = rand() % maxRandomValue + 1;

				it = std::find(selectedValues.begin(), selectedValues.end(), randomValue);
				if (it == selectedValues.end()) {
					selectedValues.push_back(randomValue);
					//std::cout << randomValue << std::endl;
				}
			} while (selectedValues.size()!=maxSelectedValues);
			std::sort(selectedValues.begin(),selectedValues.end());
			//std::for_each(selectedValues.begin(), selectedValues.end(),myobject);
			return selectedValues;
		}
		
		void selectRandomImages(std::vector<int> counterImagesArray,int maxImages,std::string filename) {
			
			std::string path = result_directory +"/"+ filename;
			enum counterIMAGES { CENTER, UP, DOWN, RIGHT, LEFT, TV,GOOD_POSE,BAD_POSE };
			std::vector<int> randomImagesValues;
			std::vector<int>::iterator it;
			std::string folder;
			cv::Mat img;
			for (int i = counterIMAGES::CENTER; i <= counterIMAGES::BAD_POSE;i++) {
				switch (i)
				{
				case(0):folder = "center";
					break;
				case(1):folder = "up";
					break;
				case(2):folder = "down";
					break;
				case(3):folder = "right";
					break;
				case(4):folder = "left";
					break;
				case(5):folder = "tv";
					break;
				case(6):folder = "good";
					break;
				case(7):folder = "bad";
					break;
				}
				randomImagesValues = getRandomValues(counterImagesArray.at(i),maxImages);
				for (it = randomImagesValues.begin(); it != randomImagesValues.end(); ++it) {
					if (*it == 0)
						continue;
					img = cv::imread(path+"/"+folder+"/img"+ std::to_string(*it) + ".jpg");
					cv::imwrite(path+ "/img" +std::to_string(*it) +"_"+folder+ ".jpg", img);
					//std::cout<<"Imagen "+std::to_string(*it)+" guardada en carpeta "+folder <<std::endl;
				}
			}
			deleteFolders(filename);
			
		}

		Status processVideo(ServerContext* context, ServerReader<Image>* reader, Response* reply) override {
			
			//Py_Initialize();
			//python_funs::initializePythonModule(FLAGS_face_features_scaler, FLAGS_face_classifier);

			std::vector<int> counterImagesArray(8);

			Image image;
			op::Wrapper<std::vector<op::Datum>> opWrapper;
			std::ofstream  openPose_output_file; //OPENPOSE output file
			std::ofstream *rap_output_file = new std::ofstream(); //RAP putput file

			// Procesa el primer request para obtener el nombre del archivo con el que se guardara
			reader->Read(&image);
			cv::Size resolution = getSizeImage(image);
			std::string resolution_flags =  std::to_string(resolution.width)+"x"+ std::to_string(resolution.height);
			
			std::string file(image.file_name());
			std::cout << "ESTUDIANTE: " << file << std::endl;

			if (FLAGS_record_video)
				audience::recordVideo(file);
			if (FLAGS_show_audience)
				audience::showAudience();

			//openPose_output_file.open(FLAGS_write_csv_file);
			openPose_output_file.open("./" + file + ".csv");
			// logging_level
			op::check(0 <= FLAGS_logging_level && FLAGS_logging_level <= 255, "Wrong logging_level value.", __LINE__, __FUNCTION__, __FILE__);
			op::ConfigureLog::setPriorityThreshold((op::Priority)FLAGS_logging_level);
			// op::ConfigureLog::setPriorityThreshold(op::Priority::None); // To print all logging messages

			op::log("Starting pose estimation demo.", op::Priority::High);
			const auto timerBegin = std::chrono::high_resolution_clock::now();

			// Applying user defined configuration - Google flags to program variables
			// outputSize
			const auto outputSize = op::flagsToPoint(resolution_flags, "1296x972");
			//const auto outputSize = op::flagsToPoint(FLAGS_resolution, "1296x972");
			// netInputSize
			const auto netInputSize = op::flagsToPoint(FLAGS_net_resolution, "656x368");
			// faceNetInputSize
			const auto faceNetInputSize = op::flagsToPoint(FLAGS_face_net_resolution, "368x368 (multiples of 16)");
			// handNetInputSize
			const auto handNetInputSize = op::flagsToPoint(FLAGS_hand_net_resolution, "368x368 (multiples of 16)");
			// poseModel
			const auto poseModel = op::flagsToPoseModel(FLAGS_model_pose);
			// keypointScale
			const auto keypointScale = op::flagsToScaleMode(FLAGS_keypoint_scale);
			// heatmaps to add
			const auto heatMapTypes = op::flagsToHeatMaps(FLAGS_heatmaps_add_parts, FLAGS_heatmaps_add_bkg, FLAGS_heatmaps_add_PAFs);
			op::check(FLAGS_heatmaps_scale >= 0 && FLAGS_heatmaps_scale <= 2, "Non valid `heatmaps_scale`.", __LINE__, __FUNCTION__, __FILE__);
			const auto heatMapScale = (FLAGS_heatmaps_scale == 0 ? op::ScaleMode::PlusMinusOne: (FLAGS_heatmaps_scale == 1 ? op::ScaleMode::ZeroToOne : op::ScaleMode::UnsignedChar));
			op::log("", op::Priority::Low, __LINE__, __FUNCTION__, __FILE__);

			// *************** NO SE REQUERE EL USO DE ESTE OBJETO (producerSharedPtr) YA QUE EL INPUT ES VIA STREAMING ********************************************
			// Producer (use default to disable any input) 
			//const auto producerSharedPtr = op::flagsToProducer(FLAGS_image_dir, FLAGS_video, FLAGS_camera, FLAGS_camera_resolution, FLAGS_camera_fps);
			//const op::WrapperStructInput wrapperStructInput{ producerSharedPtr, FLAGS_frame_first, FLAGS_frame_last, FLAGS_process_real_time,FLAGS_frame_flip, FLAGS_frame_rotate, FLAGS_frames_repeat };
			
			const op::WrapperStructInput wrapperStructInput{ NULL, FLAGS_frame_first, FLAGS_frame_last, FLAGS_process_real_time,FLAGS_frame_flip, FLAGS_frame_rotate, FLAGS_frames_repeat };
			// Initializing the user custom classes
			// Frames producer (e.g. video, webcam, ...)
			auto wUserInput2 = std::make_shared<GRPC_Input>(reader);
			// Processing
			auto wUserPostProcessing = std::make_shared<HeadPoseWorker>(&openPose_output_file, rap_output_file, result_directory, file, &counterImagesArray, &mutex_for_python);
			// GUI (Display)
			auto wUserOutput = std::make_shared<HeadPoseOutput>(&openPose_output_file, FLAGS_write_images_path, FLAGS_write_image_prefix, file);
			
			// Add custom input
			const auto workerInputOnNewThread = false;
			opWrapper.setWorkerInput(wUserInput2, workerInputOnNewThread);

			// Add custom processing
			const auto workerProcessingOnNewThread = false;
			opWrapper.setWorkerPostProcessing(wUserPostProcessing, workerProcessingOnNewThread);
			// Add custom output
			const auto workerOutputOnNewThread = true;
			opWrapper.setWorkerOutput(wUserOutput, workerOutputOnNewThread);
			// Configure OpenPose
			const op::WrapperStructPose wrapperStructPose{ netInputSize, outputSize, keypointScale, FLAGS_num_gpu,
				FLAGS_num_gpu_start, FLAGS_scale_number, (float)FLAGS_scale_gap,
				op::flagsToRenderMode(FLAGS_render_pose), poseModel,
				!FLAGS_disable_blending, (float)FLAGS_alpha_pose,
				(float)FLAGS_alpha_heatmap, FLAGS_part_to_show, FLAGS_model_folder,
				heatMapTypes, heatMapScale, (float)FLAGS_render_threshold };
			// Face configuration (use op::WrapperStructFace{} to disable it)
			const op::WrapperStructFace wrapperStructFace{ FLAGS_face, faceNetInputSize, op::flagsToRenderMode(FLAGS_face_render, FLAGS_render_pose),
				(float)FLAGS_face_alpha_pose, (float)FLAGS_face_alpha_heatmap, (float)FLAGS_face_render_threshold };
			// Hand configuration (use op::WrapperStructHand{} to disable it)
			const op::WrapperStructHand wrapperStructHand{};
			// Consumer (comment or use default argument to disable any output)
			const bool displayGui = false;
			const bool guiVerbose = false;
			const bool fullScreen = false;
			const op::WrapperStructOutput wrapperStructOutput{ displayGui, guiVerbose, fullScreen, FLAGS_write_keypoint,
				op::stringToDataFormat(FLAGS_write_keypoint_format), FLAGS_write_keypoint_json,
				FLAGS_write_coco_json, FLAGS_write_images, FLAGS_write_images_format, FLAGS_write_video,
				FLAGS_write_heatmaps, FLAGS_write_heatmaps_format };
			// Configure wrapper
			opWrapper.configure(wrapperStructPose, wrapperStructFace, wrapperStructHand, op::WrapperStructInput{}, wrapperStructOutput);
			// Set to single-thread running (e.g. for debugging purposes)
			// opWrapper.disableMultiThreading();

			op::log("Starting thread(s)", op::Priority::High);

			opWrapper.exec();
			audience::closeAudience();
			audience::stopRecording();

			wUserPostProcessing->close_video();
			cv::destroyWindow(file);

			selectRandomImages(counterImagesArray, 3, file);

			//mutex_for_python.lock();
			//sendImagesToServer(result_directory, file);
		    //mutex_for_python.unlock();

			//mutex_for_python.lock();
			//sendCSVToServer(result_directory, file, globals::filename, rap_output_file);
			//mutex_for_python.unlock();

			senDataToServer(result_directory, file, globals::filename, rap_output_file);
		   const auto now = std::chrono::high_resolution_clock::now();
		   const auto totalTimeSec = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(now - timerBegin).count() * 1e-9;
		   const auto message = "Real-time pose estimation demo successfully finished. Total time: " + std::to_string(totalTimeSec) + " seconds.";
		   std::cout << "**DONE PROCESSING ESTUDIANTES " << file << std::endl;
		   op::log(message, op::Priority::High);
		   //std::cout << counterImagesArray.at(0)<< counterImagesArray.at(1) << counterImagesArray.at(2);
		   reply->set_status("Ok");
		   return Status::OK;
	   }
};


void RunServer() {
	std::string server_address("0.0.0.0:50052");
	FeatureExtractionImpl service;

	ServerBuilder builder;
	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());//sslservercredentials
	// Register "service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *synchronous* service.
	builder.RegisterService(&service);
	// Finally assemble the server.
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;
	
	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();
}

int openPoseTutorialThread2()
{
	
	if (!boost::filesystem::exists(result_directory))
	{
		bool success = boost::filesystem::create_directories(result_directory);

		if (!success)
		{
			std::cout << "Failed to create a directory..." << result_directory << std::endl;
		}
	}
	
	RunServer();

	return 0;
}


int main(int argc, char *argv[])
{
	//std::string command("python example.py ");
	//std::string f = "python example.py " + result_directory+ " ";
	//system(f.c_str());
	// Initializing google logging (Caffe uses it for logging)
	google::InitGoogleLogging("openPoseTutorialThread2");

	// Parsing command line flags
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	//Starting python interpreter and predictor module
	_putenv_s("PYTHONPATH", ".");
	Py_Initialize();
	python_funs::initializePythonModule(FLAGS_face_features_scaler, FLAGS_face_classifier);


	/*if (FLAGS_mqtt) {
		mqtt::connect_options connOpts("james", "james");
		connOpts.set_keep_alive_interval(20);
		connOpts.set_clean_session(true);
		
		//configs::client = new mqtt::async_client(SERVER_ADDRESS, CLIENT_ID);

		callback cb(*globals::client, connOpts);
		globals::client->set_callback(cb);

		// Start the connection.
		// When completed, the callback will subscribe to topic.
		try {
			std::cout << "Connecting to the MQTT server..." << std::flush;
			globals::client->connect(connOpts, nullptr, cb);
		}
		catch (const mqtt::exception&) {
			std::cerr << "\nERROR: Unable to connect to MQTT server: '"
				<< SERVER_ADDRESS << "'" << std::endl;
			return 1;
		}		
		// Running openPoseTutorialThread2
		int res = openPoseTutorialThread2();

		try {
			std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
			globals::client->disconnect()->wait();
			std::cout << "OK" << std::endl;
		}
		catch (const mqtt::exception& exc) {
			std::cerr << exc.what() << std::endl;
			return 1;
		}
		return res;
	}	*/
	if (!FLAGS_no_streaming_input)
		return openPoseTutorialThread2();
	else
		return openPoseNoGrpcThread(mutex_for_python, result_directory);
}
