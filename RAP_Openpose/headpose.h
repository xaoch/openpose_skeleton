#pragma once
#include <openpose/core/headers.hpp>
#include <boost\python.hpp>
#include <boost\filesystem.hpp>
#include <thread>
#include <mutex>
#include <iomanip>
#include <ctime>
#include <direct.h>
#include <stdio.h>
#include "config.h"
#include "python_funs.h"
#include <windows.h>
namespace python = boost::python;
#define TO_DEGREES 180/3.141592654
#define HAND_MID_SPINE_THRESHOLD 100
#define HAND_DISTANCE_THRESHOLD 80
namespace filesystem = boost::filesystem;

// This class can be implemented either as a template or as a simple class given
// that the user usually knows which kind of data he will move between the queues,
// in this case we assume a std::shared_ptr of a std::vector of op::Datum
class HeadPoseWorker : public op::Worker<std::shared_ptr<std::vector<op::Datum>>>
{
public:
	HeadPoseWorker(std::ofstream *_csvfile, std::ofstream *_resultscsvfile, std::string _result_directory, std::string _file, std::vector<int> *counterImagesArray, std::mutex *mutex_for_python) : training_features_file{ _csvfile }, rap_csv_file{ _resultscsvfile }, result_directory{ _result_directory + "/" }, fileName{ _file }, counterImagesArray{ counterImagesArray }, mutex_for_python{ mutex_for_python }
	{
		if (training_features_file->is_open()) {
			*training_features_file << "frame,person,angle_leye_lear,angle_reye_rear,angle_leye_nose,angle_reye_nose,angle_lear_nose,angle_rear_nose,"
				"d_leye_lear,d_leye_reye,d_leye_nose,d_reye_rear,d_reye_nose,d_nose_neck,neck_x,lear,rear\n";			
		}
		//initializePythonModule();
		if (fileName.compare("")) {
			
			createFolders(fileName);
			rap_csv_file->open(result_directory + fileName + "/" + globals::filename + ".csv");
			*rap_csv_file << "frame,looks,positions\n";
		}
	}

	void initializationOnThread() {}

	float calcAngle(float p1_x, float p1_y, float p2_x, float p2_y) {
		if (p1_x == 0 || p1_y == 0 || p2_x == 0 || p2_y == 0)
			return 1000;
		return atan2(p1_y - p2_y, p1_x - p2_x) * TO_DEGREES;

	}

	float distance(float p1_x, float p1_y, float p2_x, float p2_y, float dist) {
		if (p1_x == 0 || p2_x == 0 || dist == 0)
			return -1;
		return (pow(p1_x - p2_x, 2) + pow(p1_y - p2_y, 2)) / dist;
	}

	/*void initializePythonModule() {
		try
		{
			python::object my_python_module = python::import("predictor");
			python::object loadPredictorFun = my_python_module.attr("load_predictor");
			python::object loadResult = loadPredictorFun(FLAGS_face_features_scaler, FLAGS_face_classifier);
			//pythonPredictFun = my_python_module.attr("predict");
		}
		catch (const python::error_already_set&)
		{
			PyErr_Print();
		}
	}*/

	void createFolders(std::string filename) {
		std::string path = result_directory + filename;
		//filesystem::remove_all(path);
		_mkdir(path.c_str());
		std::string actualFile = path + "/center";
		_mkdir(actualFile.c_str());
		actualFile = path + "/up";
		_mkdir(actualFile.c_str());
		actualFile = path + "/down";
		_mkdir(actualFile.c_str());
		actualFile = path + "/right";
		_mkdir(actualFile.c_str());
		actualFile = path + "/left";
		_mkdir(actualFile.c_str());
		actualFile = path + "/tv";
		_mkdir(actualFile.c_str());
		actualFile = path + "/good";
		_mkdir(actualFile.c_str());
		actualFile = path + "/bad";
		_mkdir(actualFile.c_str());
		
	}


	void captureFacePoseImages(cv::Mat img, cv::Mat showimg, std:: string actualOrientation, std:: string *lastOrientation , int *actualFramesForCaptures, int mode , int x,int y,int width,int height) {
		
		//Mode 0 Face, Mode 1 Pose
		if (mode != 0 && mode != 1)return  ;
		
		int countImg = 0;
		int countImageEnum = 0;
		if (actualOrientation.compare(*lastOrientation)) {
			//std::cout << "Primera"<< *lastFaceOrientation <<std::endl;
			*actualFramesForCaptures = 0;
			*lastOrientation = actualOrientation;
			return;
		}
		*actualFramesForCaptures = *actualFramesForCaptures+1;
		if (*actualFramesForCaptures != maxFramesForCapture) {
			//std::cout << "Segunda"<< *actualFramesForCaptures << std::endl;
			*lastOrientation = actualOrientation;
			return;
		}
		*actualFramesForCaptures = 0;
		*lastOrientation = actualOrientation;
		
		if (!actualOrientation.compare("center"))
			countImageEnum = counterIMAGES::CENTER;
		else if (!actualOrientation.compare("up"))
			countImageEnum = counterIMAGES::UP;
		else if (!actualOrientation.compare("down"))
			countImageEnum = counterIMAGES::DOWN;
		else if (!actualOrientation.compare("right"))
			countImageEnum = counterIMAGES::RIGHT;
		else if (!actualOrientation.compare("left"))
			countImageEnum = counterIMAGES::LEFT;
		else if (!actualOrientation.compare("tv"))
			countImageEnum = counterIMAGES::TV;
		else if (!actualOrientation.compare("good"))
			countImageEnum = counterIMAGES::GOOD_POSE;
		else if (!actualOrientation.compare("bad"))
			countImageEnum = counterIMAGES::BAD_POSE;
		else
			return;
		if (mode == 0) {
			
			//cv::rectangle(img,rectangleP1,rectangleP2,cv::Scalar(255, 0, 255));
			// Condiciones que debe cumplir el ROI
			if (0 < x && 0 < width && x + width < img.cols && 0 < y && 0 < height && y + height < img.rows) {
				try {
					// box within the image plane
					cv::putText(showimg, "Imagen capturada (FACE)", cv::Point(20, 80), 1, 1, cv::Scalar(255, 0, 0), 2);
					cv::Rect roi(x, y, width, height);
					//cv::Rect roi(rectangleP1, rectangleP2);
					//std::cout << roi << std::endl;
					img = img(roi);
					cv::resize(img, img, cv::Size(200, 200), 0, 0, cv::INTER_AREA);
				}
				catch (cv::Exception& e) {
					const char* err_msg = e.what();
					std::cout << "exception caught: " << err_msg << std::endl;
					return;
				}
			}
			else
				return;
			
		}
		counterImagesArray->at(countImageEnum)++;
		countImg = counterImagesArray->at(countImageEnum);

		cv::putText(showimg, "Imagen capturada (POSE)", cv::Point(20, 100), 1, 1, cv::Scalar(0, 255, 0), 2);
		//std::cout << "Imagen capturada"<<actualOrientation<<std::endl;
		cv::imwrite(result_directory+fileName+"/"+ actualOrientation+"/img" + std::to_string(countImg)+".jpg", img);
	}


	void generateVideoFile(cv::Mat img) {
		if (!resultVideo.isOpened()) {
			//int codec = CV_FOURCC('M', 'J', 'P', 'G');
			int codec = CV_FOURCC('W', 'M', 'V', '2');
			//int codec = CV_FOURCC('X', '2', '6', '4');
			double fps = 5.0;              
			//resultVideo.open("./name.avi", codec, fps, img.size(), true);
			resultVideo.open(result_directory+fileName+"/video.avi", codec, fps, img.size(), true);
			//std::cout << "open result video " << result_directory + fileName + "/video.avi" << std::endl;

		}else {
			//std::cout << "Write video" << std::endl;
			resultVideo.write(img);
		}
	}

	void writeFaceFeatures(std::vector<float> &face_features, int person, int count) {
		if (this->training_features_file->is_open()) {
			*this->training_features_file << count << "," << person;
			for each (float feature in face_features)
			{
				*training_features_file << "," << feature;
			}
			*training_features_file << "\n";
		}
	}

	
	void writeToRapCsv(int person_index, int posture, std::string face) {
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);
		int gaze = 0;
		if (posture != POSTURES::GOOD)
			posture = 0;
		if (!face.compare("center"))
			gaze = 1;
		//*this->results_csv_file << std::put_time(&tm, "%H:%M:%S") << "," << person_index << "," << posture << "," << face << "\n";
		//*this->results_csv_file << frame << "," << posture << "," << face << "\n";
		//*results_csv_file << frame << "," << gaze << "," << posture << "\n";
		*rap_csv_file << frame << "," << face << "," << posture << "\n";
		//*this->results_csv_file <<  "," << person_index << "," << posture << "," << face << "\n";
		//this->results_csv_file->flush();
	}

	std::string predict_face_orientation(const std::vector<float> &v) {
		try {			
			python::object result = python_funs::pythonPredictFun(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14]);
			return python::extract<std::string>(result);
		}
		catch (const python::error_already_set&) {
			PyErr_Print();
			return "Error";
		}

	}

	//Selects the id of biggest skeleton found in a frame based on size of body
	int select_biggest_skeleton(const op::Array<float> &keypoints) {
		int max_id = 0;
		int max_size = 0;
		for (int i = 0; i < keypoints.getSize(0); i++) {
			float rhip_y = keypoints.at(std::vector<int>{i, 8, 1});
			float lhip_y = keypoints.at(std::vector<int>{i, 11, 1});
			float neck_y = keypoints.at(std::vector<int>{i, 1, 1});
			int size = 0;
			if (neck_y != 0 && (rhip_y != 0 || lhip_y != 0)) {
				size = (rhip_y + lhip_y) / 2 - neck_y;
				if (size > max_size) {
					max_size = size;
					max_id = i;
				}
			}			
		}
		return max_id;
	}

	int posture_classifier(const op::Array<float> &keypoints, int person_index, std::string face_orientation, float head_height)
	{
		float rwrist_y = keypoints.at(std::vector<int>{person_index, 4, 1}); float rwrist_x = keypoints.at(std::vector<int>{person_index, 4, 0});
		float lwrist_y = keypoints.at(std::vector<int>{person_index, 7, 1}); float lwrist_x = keypoints.at(std::vector<int>{person_index, 7, 0});
		float rhip_y = keypoints.at(std::vector<int>{person_index, 8, 1});
		float lhip_y = keypoints.at(std::vector<int>{person_index, 11, 1});
		float neck_y = keypoints.at(std::vector<int>{person_index, 1, 1});
		float nose_y = keypoints.at(std::vector<int>{person_index, 0, 1});
		float rshoulder_x = keypoints.at(std::vector<int>{person_index, 2, 0}); float lshoulder_x = keypoints.at(std::vector<int>{person_index, 5, 0});
		if (rshoulder_x != 0 && lshoulder_x != 0 && (lshoulder_x < rshoulder_x) > 0) //Persona de espaldas
			return POSTURES::BAD;
		if (rhip_y == 0 && lhip_y == 0)
			return POSTURES::NOT_DETECTED;
		if (lwrist_y == 0)
			lwrist_y = rwrist_y;
		if (rwrist_y == 0)
			rwrist_y = lwrist_y;
		if (rwrist_y == 0)
			return POSTURES::NOT_DETECTED;

		float hand_distance_threshold = neck_y - nose_y;
		float spinebase = (rhip_y + lhip_y) / 2;
		float spinemid = ((3*spinebase) + neck_y)/4;
		float normalizer = (head_height > 0) ? head_height : HAND_MID_SPINE_THRESHOLD;
		if ((lwrist_y < spinemid - (HAND_MID_SPINE_THRESHOLD/head_height))|| rwrist_y < spinemid - (HAND_MID_SPINE_THRESHOLD/head_height)){// && face_orientation.compare("tv")) {
			if (rwrist_x != 0 && lwrist_x != 0 && abs(rwrist_x - lwrist_x) < hand_distance_threshold) { //Manos juntas
				return POSTURES::BAD;
			}
			return POSTURES::GOOD;
			
		}			
		return POSTURES::BAD;
		//else if ((lwrist_y > spinemid && rwrist_y > spinemid))
		//	return POSTURES::BAD;
		//else if ((lwrist_y > spinebase && rwrist_y > spinebase))
		//	return POSTURES::BAD;
		//else if ((lwrist_y < spinemid || rwrist_y < spinemid))//return POSTURES::NORMAL;
		//	return POSTURES::GOOD;
		//else
		//	return POSTURES::BAD;
		//return -1;
	}

	void close_video() {
		resultVideo.release();
	}

	void toggle_recording() {
		this->recording = globals::record;
		if (globals::record) {
			rap_csv_file->open(result_directory + fileName + "/" + globals::filename + ".csv");
			*rap_csv_file << "frame,looks,positions\n";
		}
		else {
			rap_csv_file->close();
		}
		if (FLAGS_mqtt)
			globals::client->publish(mqtt::make_message("ack_rap_status", std::to_string(this->recording)));
	}

	void work(std::shared_ptr<std::vector<op::Datum>>& datumsPtr)
	{
		try
		{
			// User's processing here
			// datum.cvInputData: initial cv::Mat obtained from the frames producer (video, webcam, etc.)
			// datum.cvOutputData: final cv::Mat to be displayed
			if (datumsPtr != nullptr) {
				frame++;
				int lear = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "LEar");
				int rear = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "REar");
				int reye = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "REye");
				int leye = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "LEye");
				int nose = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "Nose");
				int neck = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "Neck");				
				for (auto& datum : *datumsPtr) {
				
					auto Rectangles = datum.faceRectangles;
					auto keypoints = datum.poseKeypoints;
					int keypoints_id = 0;
					if (keypoints.getSize(0) >= 1) {
						keypoints_id = select_biggest_skeleton(keypoints);
						if (Rectangles.size() > 0 && Rectangles.at(keypoints_id).height != 0) {

							/*cv::rectangle(datum.cvInputData,
								cv::Point(Rectangles.at(keypoints_id).x, Rectangles.at(keypoints_id).y),
								cv::Point(Rectangles.at(keypoints_id).x + Rectangles.at(keypoints_id).width, Rectangles.at(keypoints_id).y + Rectangles.at(keypoints_id).height),
								cv::Scalar(255, 0, 255)
							);*/
							cv::putText(datum.cvOutputData, "HEAD", cv::Point(20, 60), 1, 1, cv::Scalar(0, 255, 255));
							head_height = Rectangles.at(keypoints_id).height;
							//std::cout << "Head size: " << head_height << std::endl;
						}
						else {
							cv::putText(datum.cvOutputData, "NO HEAD", cv::Point(20, 60), 1, 1, cv::Scalar(255, 0, 0));
						}					

						std::vector<float> v;
						float nose_x = keypoints.at(std::vector<int>{keypoints_id, nose, 0}); float nose_y = keypoints.at(std::vector<int>{keypoints_id, nose, 1});
						float leye_x = keypoints.at(std::vector<int>{keypoints_id, leye, 0}); float leye_y = keypoints.at(std::vector<int>{keypoints_id, leye, 1});
						float reye_x = keypoints.at(std::vector<int>{keypoints_id, reye, 0}); float reye_y = keypoints.at(std::vector<int>{keypoints_id, reye, 1});
						float lear_x = keypoints.at(std::vector<int>{keypoints_id, lear, 0}); float lear_y = keypoints.at(std::vector<int>{keypoints_id, lear, 1});
						float rear_x = keypoints.at(std::vector<int>{keypoints_id, rear, 0}); float rear_y = keypoints.at(std::vector<int>{keypoints_id, rear, 1});
						float neck_x = keypoints.at(std::vector<int>{keypoints_id, neck, 0}); float neck_y = keypoints.at(std::vector<int>{keypoints_id, neck, 1});

						v.push_back(calcAngle(leye_x, leye_y, lear_x, lear_y));
						v.push_back(calcAngle(reye_x, reye_y, rear_x, rear_y));
						v.push_back(calcAngle(leye_x, leye_y, nose_x, nose_y));
						v.push_back(calcAngle(reye_x, reye_y, nose_x, nose_y));
						v.push_back(calcAngle(lear_x, lear_y, nose_x, nose_y));
						v.push_back(calcAngle(rear_x, rear_y, nose_x, nose_y));
						v.push_back(distance(leye_x, leye_y, lear_x, lear_y, head_height));
						v.push_back(distance(leye_x, leye_y, reye_x, reye_y, head_height));
						v.push_back(distance(leye_x, leye_y, nose_x, nose_y, head_height));
						v.push_back(distance(reye_x, reye_y, rear_x, rear_y, head_height));
						v.push_back(distance(reye_x, reye_y, nose_x, nose_y, head_height));
						v.push_back((nose_y - neck_y) / head_height);
						v.push_back(neck_x);
						v.push_back(lear_x == 0 ? 0 : 1);
						v.push_back(rear_x == 0 ? 0 : 1);

						mutex_for_python->lock();
						std::string face_orientation = this->predict_face_orientation(v);
						mutex_for_python->unlock();
								
						std::string postureString = "";
						int posture = this->posture_classifier(keypoints, keypoints_id, face_orientation, head_height);
						if (posture == POSTURES::GOOD) {
							postureString = "good";
							cv::putText(datum.cvOutputData, "GOOD", cv::Point(100, 40), 1, 1, cv::Scalar(0, 255, 0), 2);
							cv::putText(datum.cvInputData, "GOOD", cv::Point(100, 40), 1, 1, cv::Scalar(0, 255, 0), 2);
						}
						else if (posture == POSTURES::BAD) {
							postureString = "bad";
							cv::putText(datum.cvOutputData, "BAD", cv::Point(100, 40), 1, 1, cv::Scalar(0, 0, 255), 2);
							cv::putText(datum.cvInputData, "BAD", cv::Point(100, 40), 1, 1, cv::Scalar(0, 0, 255), 2);
						}
						else if (posture == POSTURES::NORMAL) {
							postureString = "good";
							cv::putText(datum.cvOutputData, "GOOD", cv::Point(100, 40), 1, 1, cv::Scalar(255, 255, 0), 2);
							cv::putText(datum.cvInputData, "GOOD", cv::Point(100, 40), 1, 1, cv::Scalar(0, 255, 0), 2);
						}
						else if (posture == POSTURES::NOT_DETECTED) {
						}
						cv::putText(datum.cvOutputData, "head:", cv::Point(20, 20), 1, 1, cv::Scalar(255, 0, 0), 2);
						cv::putText(datum.cvOutputData, "posture:", cv::Point(20, 40), 1, 1, cv::Scalar(255, 0, 0), 2);
						cv::putText(datum.cvOutputData, face_orientation, cv::Point(80, 20), 1, 1, cv::Scalar(255, 255, 0), 2);

						cv::putText(datum.cvInputData, "head:", cv::Point(20, 20), 1, 1, cv::Scalar(255, 0, 0), 2);
						cv::putText(datum.cvInputData, "posture:", cv::Point(20, 40), 1, 1, cv::Scalar(255, 0, 0), 2);
						cv::putText(datum.cvInputData, face_orientation, cv::Point(80, 20), 1, 1, cv::Scalar(255, 255, 0), 2);

						if (globals::record != this->recording)
							this->toggle_recording();

						this->writeToRapCsv(keypoints_id, posture, face_orientation);
						this->writeFaceFeatures(v, keypoints_id, count);
						//std::cout << Rectangles.at(0).x << " " << Rectangles.at(0).y << " " << Rectangles.at(0).width << " " << Rectangles.at(0).height << std::endl;

						// Si detetecta un rostro, realiza la captura del frame de la mirada
						if (Rectangles.size() > 0) {
							captureFacePoseImages(datum.cvInputData, datum.cvOutputData, face_orientation, &lastFaceOrientation, &actualFramesForCaptureFace, 0,
								Rectangles.at(0).x, Rectangles.at(0).y, Rectangles.at(0).width, Rectangles.at(0).height);
						}
						// Se realiza la captura del frame de la postura
						captureFacePoseImages(datum.cvInputData, datum.cvOutputData, postureString, &lastPose, &actualFramesForCapturePose, 1, 0, 0, 0, 0);

						//datum.face_features.push_back(v);
					}
					generateVideoFile(datum.cvInputData);
				}
				
			}
			count++;
		}
		catch (const std::exception& e)
		{
			op::log("Some kind of unexpected error happened.");
			this->stop();
			op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
		}
	}
//public:
	//cv::VideoWriter resultVideo;
private:
	//python::object pythonPredictFun;
	cv::VideoWriter resultVideo;
	std::string fileName;
	std::ofstream *training_features_file, *rap_csv_file;
	long count=0;
	float head_height = 1;
	float spine_hip_distance = 20;
	enum POSTURES {BAD, GOOD, NORMAL, NOT_DETECTED};
	bool recording = false;
	int frame = 0;
	//Variables para la captura de imagenes
	int maxFramesForCapture = 5*2;//3 segundos (5fps)
	int actualFramesForCaptureFace = 0;
	//int maxFramesForCapturePose = 5*5;
	int actualFramesForCapturePose= 0;
	std::string lastFaceOrientation = "";
	std::string lastPose = "";
	bool createdFolders = false;
	enum counterIMAGES { CENTER,UP,DOWN,RIGHT,LEFT,TV,GOOD_POSE,BAD_POSE};
	//int counterImagesArray[8] = {0,0,0,0,0,0,0};
	std::vector<int> *counterImagesArray;
	std::mutex *mutex_for_python;
	//std::mutex mutex;
	std::string result_directory;
};