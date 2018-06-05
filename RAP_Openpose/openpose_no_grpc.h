#pragma once

#include <openpose/core/headers.hpp>
#include <openpose/headers.hpp>
#include "params.h"
#include "headpose.h"
#include "headpose_output.h"
#include <mutex>
#include "audience.h"

int openPoseNoGrpcThread(std::mutex &mutex_for_python, std::string result_directory)
{
	//std::ofstream  myfile;
	//myfile.open(FLAGS_write_face_keypoints_file);
	std::vector<int> counterImagesArray(8);
	std::ofstream  face_features_file; //OPENPOSE output file
	std::ofstream *rap_output_file = new std::ofstream(); //RAP putput file
	std::string file = "recording";

	if (FLAGS_face_training_features) {
		face_features_file.open(FLAGS_write_face_features_file);		
		op::log("Writing face training to " + FLAGS_write_face_features_file);
	}
	if (FLAGS_record_video)
		audience::recordVideo(file);
	if (FLAGS_show_audience)
		audience::showAudience();

	// logging_level
	op::check(0 <= FLAGS_logging_level && FLAGS_logging_level <= 255, "Wrong logging_level value.", __LINE__, __FUNCTION__, __FILE__);
	op::ConfigureLog::setPriorityThreshold((op::Priority)FLAGS_logging_level);
	// op::ConfigureLog::setPriorityThreshold(op::Priority::None); // To print all logging messages

	op::log("Starting pose estimation demo.", op::Priority::High);
	const auto timerBegin = std::chrono::high_resolution_clock::now();

	// Applying user defined configuration - Google flags to program variables
	// outputSize
	const auto outputSize = op::flagsToPoint(FLAGS_resolution, "1280x720");
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
	const auto heatMapScale = (FLAGS_heatmaps_scale == 0 ? op::ScaleMode::PlusMinusOne
		: (FLAGS_heatmaps_scale == 1 ? op::ScaleMode::ZeroToOne : op::ScaleMode::UnsignedChar));
	op::log("", op::Priority::Low, __LINE__, __FUNCTION__, __FILE__);

	// Initializing the user custom classes
	// Frames producer (e.g. video, webcam, ...)
	//auto wUserInput = std::make_shared<WUserInput>(FLAGS_image_dir);

	// Producer (use default to disable any input)
	const auto producerSharedPtr = op::flagsToProducer(FLAGS_image_dir, FLAGS_video, FLAGS_camera, FLAGS_camera_resolution, FLAGS_camera_fps);
	const op::WrapperStructInput wrapperStructInput{ producerSharedPtr, FLAGS_frame_first, FLAGS_frame_last, FLAGS_process_real_time,
		FLAGS_frame_flip, FLAGS_frame_rotate, FLAGS_frames_repeat };
	
	// Processing
	auto wUserPostProcessing = std::make_shared<HeadPoseWorker>(&face_features_file, rap_output_file, result_directory, file, &counterImagesArray, &mutex_for_python);
	// GUI (Display)
	auto wUserOutput = std::make_shared<HeadPoseOutput>(&face_features_file, FLAGS_write_images_path, FLAGS_write_image_prefix, file);

	op::Wrapper<std::vector<op::Datum>> opWrapper;

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
	opWrapper.configure(wrapperStructPose, wrapperStructFace, wrapperStructHand, wrapperStructInput, wrapperStructOutput);
	// Set to single-thread running (e.g. for debugging purposes)
	// opWrapper.disableMultiThreading();

	op::log("Starting thread(s)", op::Priority::High);
	// Two different ways of running the program on multithread environment
	// // Option a) Recommended - Also using the main thread (this thread) for processing (it saves 1 thread)
	// // Start, run & stop threads
	opWrapper.exec();  // It blocks this thread until all threads have finished
	audience::closeAudience();
	audience::stopRecording();

	wUserPostProcessing->close_video();
	if (FLAGS_face_training_features)
		face_features_file.close();
	cv::destroyWindow(file);
					   // Option b) Keeping this thread free in case you want to do something else meanwhile, e.g. profiling the GPU memory
					   // // VERY IMPORTANT NOTE: if OpenCV is compiled with Qt support, this option will not work. Qt needs the main thread to
					   // // plot visual results, so the final GUI (which uses OpenCV) would return an exception similar to:
					   // // `QMetaMethod::invoke: Unable to invoke methods with return values in queued connections`
					   // // Start threads
					   // opWrapper.start();
					   // // Profile used GPU memory
					   //     // 1: wait ~10sec so the memory has been totally loaded on GPU
					   //     // 2: profile the GPU memory
					   // std::this_thread::sleep_for(std::chrono::milliseconds{1000});
					   // op::log("Random task here...", op::Priority::High);
					   // // Keep program alive while running threads
					   // while (opWrapper.isRunning())
					   //     std::this_thread::sleep_for(std::chrono::milliseconds{33});
					   // // Stop and join threads
					   // op::log("Stopping thread(s)", op::Priority::High);
					   // opWrapper.stop();

					   // Measuring total time
	const auto now = std::chrono::high_resolution_clock::now();
	const auto totalTimeSec = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(now - timerBegin).count() * 1e-9;
	const auto message = "Real-time pose estimation demo successfully finished. Total time: " + std::to_string(totalTimeSec) + " seconds.";
	op::log(message, op::Priority::High);



	return 0;
}