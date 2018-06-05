#pragma once
#include <openpose/core/headers.hpp>
#include <openpose/headers.hpp>
#define TO_DEGREES 180/3.141592654

class HeadPoseOutput : public op::WorkerConsumer<std::shared_ptr<std::vector<op::Datum>>>
{
public:

	int count = 0;
	std::string path;
	std::string imgprefix;
	std::ofstream *csvfile;
	std::string file;
	void initializationOnThread() {}
	HeadPoseOutput(std::ofstream *_csvfile, const std::string &_path, const std::string &_imgprefix, std::string &_file) : path{ (_path + _imgprefix) }, csvfile{ _csvfile }, imgprefix{ _imgprefix }, file{_file} {
	/*	*csvfile << "frame,person,angle_leye_lear,angle_reye_rear,angle_leye_nose,angle_reye_nose,angle_lear_nose,angle_rear_nose,"
			"d_leye_lear,d_leye_reye,d_leye_nose,d_reye_rear,d_reye_nose,lear,rear,neck_x\n";*/
	}

	void writeToCsv(std::vector<float> &face_features, int person, int count) {
		*csvfile << count << "," << person;
		for each (float feature in face_features)
		{
			*csvfile << "," << feature;
		}
		*csvfile << "\n";
	}

	void workConsumer(const std::shared_ptr<std::vector<op::Datum>>& datumsPtr)
	{
		try
		{
			// User's displaying/saving/other processing here
			// datum.cvOutputData: rendered frame with pose or heatmaps
			// datum.poseKeypoints: Array<float> with the estimated pose
			if (datumsPtr != nullptr && !datumsPtr->empty())
			{
				/*if (count++ % 5 == 0)
				cv::imwrite(path + std::to_string(count) + ".png", datumsPtr->at(0).cvInputData);		*/
				/*auto face_features = &datumsPtr->at(0).face_features;
				for (int i = 0; i < face_features->size(); i++) {
				writeToCsv(face_features->at(i), i, count);
				}*/
				//count++;
				/*int lear = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "LEar");
				int rear = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "REar");
				int reye = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "REye");
				int leye = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "LEye");
				int nose = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "Nose");
				int neck = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "Neck");
				int ankle = op::poseBodyPartMapStringToKey(op::PoseModel::COCO_18, "RAnkle");
				auto keypoints = datumsPtr->at(0).poseKeypoints;
				for (int i = 0; i < keypoints.getSize(0); i++) {

				float nose_x = keypoints.at(std::vector<int>{i, nose, 0}); float nose_y = keypoints.at(std::vector<int>{i, nose, 1});
				float leye_x = keypoints.at(std::vector<int>{i, leye, 0}); float leye_y = keypoints.at(std::vector<int>{i, leye, 1});
				float reye_x = keypoints.at(std::vector<int>{i, reye, 0}); float reye_y = keypoints.at(std::vector<int>{i, reye, 1});
				float lear_x = keypoints.at(std::vector<int>{i, lear, 0}); float lear_y = keypoints.at(std::vector<int>{i, lear, 1});
				float rear_x = keypoints.at(std::vector<int>{i, rear, 0}); float rear_y = keypoints.at(std::vector<int>{i, rear, 1});
				float neck_x = keypoints.at(std::vector<int>{i, neck, 0}); float neck_y = keypoints.at(std::vector<int>{i, neck, 1});
				*csvfile << count << "," << i
				<< "," << nose_x << "," << nose_y <<  "," << keypoints.at(std::vector<int>{i, nose, 2})
				<< "," << leye_x << "," << leye_y << "," << keypoints.at(std::vector<int>{i, leye, 2})
				<< "," << reye_x << "," << reye_y << "," << keypoints.at(std::vector<int>{i, reye, 2})
				<< "," << lear_x << "," << lear_y << "," << keypoints.at(std::vector<int>{i, lear, 2})
				<< "," << rear_x << "," << rear_y << "," << keypoints.at(std::vector<int>{i, rear, 2})
				<< "," << neck_x << "," << neck_y << "," << keypoints.at(std::vector<int>{i, neck, 2})
				<< "," << calcAngle(leye_x, leye_y, lear_x, lear_y) << "," << calcAngle(reye_x, reye_y, rear_x, rear_y)
				<< "," << calcAngle(leye_x, leye_y, nose_x, nose_y) << "," << calcAngle(reye_x, reye_y, nose_x, nose_y)
				<< "," << calcAngle(lear_x, lear_y, nose_x, nose_y) << "," << calcAngle(rear_x, rear_y, nose_x, nose_y)
				<< "," << keypoints.at(std::vector<int>{i, ankle, 1}) - neck_y << "\n";
				}*/
				if (!FLAGS_no_display) {
					cv::resize(datumsPtr->at(0).cvOutputData, datumsPtr->at(0).cvOutputData, cv::Size(1296,972));
					cv::imshow(file, datumsPtr->at(0).cvOutputData);
					//cv::imshow(file, datumsPtr->at(0).cvInputData);
					cv::waitKey(2); // It displays the image and sleeps at least 1 ms (it usually sleeps ~5-10 msec to display the image)
				}
			}
		}
		catch (const std::exception& e)
		{
			op::log("Some kind of unexpected error happened.");
			this->stop();
			op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
		}
	}
};