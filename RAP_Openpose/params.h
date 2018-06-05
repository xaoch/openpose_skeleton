#pragma once
#include <gflags/gflags.h> // DEFINE_bool, DEFINE_int32, DEFINE_int64, DEFINE_uint64, DEFINE_double, DEFINE_string
// See all the available parameter options withe the `--help` flag. E.g. `./build/examples/openpose/openpose.bin --help`.
// Note: This command will show you flags for other unnecessary 3rdparty files. Check only the flags for the OpenPose
// executable. E.g. for `openpose.bin`, look for `Flags from examples/openpose/openpose.cpp:`.
// Debugging
DEFINE_int32(logging_level, 4, "The logging level. Integer in the range [0, 255]. 0 will output any log() message, while"
	" 255 will not output any. Current OpenPose library messages are in the range 0-4: 1 for"
	" low priority messages and 4 for important ones.");
// Producer
DEFINE_int32(camera, 0, "The camera index for cv::VideoCapture. Integer in the range [0, 9].");
DEFINE_string(camera_resolution, "1280x720", "Size of the camera frames to ask for.");
DEFINE_double(camera_fps, 30.0, "Frame rate for the webcam (only used when saving video from webcam). Set this value to the"
	" minimum value between the OpenPose displayed speed and the webcam real frame rate.");
DEFINE_string(video, "", "Use a video file instead of the camera. Use `examples/media/video.avi` for our default"
	" example video.");
DEFINE_string(image_dir, "", "Process a directory of images. Use `examples/media/` for our default example folder with 20"
	" images.");
DEFINE_uint64(frame_first, 0, "Start on desired frame number. Indexes are 0-based, i.e. the first frame has index 0.");
DEFINE_uint64(frame_last, -1, "Finish on desired frame number. Select -1 to disable. Indexes are 0-based, e.g. if set to"
	" 10, it will process 11 frames (0-10).");
DEFINE_bool(frame_flip, false, "Flip/mirror each frame (e.g. for real time webcam demonstrations).");
DEFINE_int32(frame_rotate, 0, "Rotate each frame, 4 possible values: 0, 90, 180, 270.");
DEFINE_bool(frames_repeat, false, "Repeat frames when finished.");
// OpenPose
DEFINE_string(model_folder, "D:/openpose/models/", "Folder path (absolute or relative) where the models (pose, face, ...) are located.");
DEFINE_string(resolution, "1280x720", "The image resolution (display and output). Use \"-1x-1\" to force the program to use the"
	" default images resolution.");
DEFINE_int32(num_gpu, -1, "The number of GPU devices to use. If negative, it will use all the available GPUs in your"
	" machine.");
DEFINE_int32(num_gpu_start, 0, "GPU device start number.");
DEFINE_int32(keypoint_scale, 0, "Scaling of the (x,y) coordinates of the final pose data array, i.e. the scale of the (x,y)"
	" coordinates that will be saved with the `write_keypoint` & `write_keypoint_json` flags."
	" Select `0` to scale it to the original source resolution, `1`to scale it to the net output"
	" size (set with `net_resolution`), `2` to scale it to the final output size (set with"
	" `resolution`), `3` to scale it in the range [0,1], and 4 for range [-1,1]. Non related"
	" with `scale_number` and `scale_gap`.");
// OpenPose Body Pose
DEFINE_string(model_pose, "COCO", "Model to be used (e.g. COCO, MPI, MPI_4_layers).");
DEFINE_string(net_resolution, "656x368", "Multiples of 16. If it is increased, the accuracy usually increases. If it is decreased,"
	" the speed increases.");
DEFINE_int32(scale_number, 1, "Number of scales to average.");
DEFINE_double(scale_gap, 0.3, "Scale gap between scales. No effect unless scale_number > 1. Initial scale is always 1."
	" If you want to change the initial scale, you actually want to multiply the"
	" `net_resolution` by your desired initial scale.");
DEFINE_bool(heatmaps_add_parts, false, "If true, it will add the body part heatmaps to the final op::Datum::poseHeatMaps array"
	" (program speed will decrease). Not required for our library, enable it only if you intend"
	" to process this information later. If more than one `add_heatmaps_X` flag is enabled, it"
	" will place then in sequential memory order: body parts + bkg + PAFs. It will follow the"
	" order on POSE_BODY_PART_MAPPING in `include/openpose/pose/poseParameters.hpp`.");
DEFINE_bool(heatmaps_add_bkg, false, "Same functionality as `add_heatmaps_parts`, but adding the heatmap corresponding to"
	" background.");
DEFINE_bool(heatmaps_add_PAFs, false, "Same functionality as `add_heatmaps_parts`, but adding the PAFs.");
DEFINE_int32(heatmaps_scale, 2, "Set 0 to scale op::Datum::poseHeatMaps in the range [0,1], 1 for [-1,1]; and 2 for integer"
	" rounded [0,255].");
// OpenPose Face
DEFINE_bool(face, true, "Enables face keypoint detection. It will share some parameters from the body pose, e.g."
	" `model_folder`.");
DEFINE_string(face_net_resolution, "368x368", "Multiples of 16. Analogous to `net_resolution` but applied to the face keypoint detector."
	" 320x320 usually works fine while giving a substantial speed up when multiple faces on the"
	" image.");
// OpenPose Hand
DEFINE_bool(hand, false, "Enables hand keypoint detection. It will share some parameters from the body pose, e.g."
	" `model_folder`.");
DEFINE_string(hand_net_resolution, "368x368", "Multiples of 16. Analogous to `net_resolution` but applied to the hand keypoint detector.");
DEFINE_int32(hand_scale_number, 1, "Analogous to `scale_number` but applied to the hand keypoint detector. Our best results"
	" were found with `hand_scale_number` = 6 and `hand_scale_range` = 0.4");
DEFINE_double(hand_scale_range, 0.4, "Analogous purpose than `scale_gap` but applied to the hand keypoint detector. Total range"
	" between smallest and biggest scale. The scales will be centered in ratio 1. E.g. if"
	" scaleRange = 0.4 and scalesNumber = 2, then there will be 2 scales, 0.8 and 1.2.");
DEFINE_bool(hand_tracking, false, "Adding hand tracking might improve hand keypoints detection for webcam (if the frame rate"
	" is high enough, i.e. >7 FPS per GPU) and video. This is not person ID tracking, it"
	" simply looks for hands in positions at which hands were located in previous frames, but"
	" it does not guarantee the same person ID among frames");
// OpenPose Rendering
DEFINE_int32(part_to_show, 0, "Part to show from the start.");
DEFINE_bool(disable_blending, false, "If blending is enabled, it will merge the results with the original frame. If disabled, it"
	" will only display the results.");
// OpenPose Rendering Pose
DEFINE_double(render_threshold, 0.05, "Only estimated keypoints whose score confidences are higher than this threshold will be"
	" rendered. Generally, a high threshold (> 0.5) will only render very clear body parts;"
	" while small thresholds (~0.1) will also output guessed and occluded keypoints, but also"
	" more false positives (i.e. wrong detections).");
DEFINE_int32(render_pose, 2, "Set to 0 for no rendering, 1 for CPU rendering (slightly faster), and 2 for GPU rendering"
	" (slower but greater functionality, e.g. `alpha_X` flags). If rendering is enabled, it will"
	" render both `outputData` and `cvOutputData` with the original image and desired body part"
	" to be shown (i.e. keypoints, heat maps or PAFs).");
DEFINE_double(alpha_pose, 0.6, "Blending factor (range 0-1) for the body part rendering. 1 will show it completely, 0 will"
	" hide it. Only valid for GPU rendering.");
DEFINE_double(alpha_heatmap, 0.7, "Blending factor (range 0-1) between heatmap and original frame. 1 will only show the"
	" heatmap, 0 will only show the frame. Only valid for GPU rendering.");
// OpenPose Rendering Face
DEFINE_double(face_render_threshold, 0.4, "Analogous to `render_threshold`, but applied to the face keypoints.");
DEFINE_int32(face_render, 0, "Analogous to `render_pose` but applied to the face. Extra option: -1 to use the same"
	" configuration that `render_pose` is using.");
DEFINE_double(face_alpha_pose, 0.6, "Analogous to `alpha_pose` but applied to face.");
DEFINE_double(face_alpha_heatmap, 0.7, "Analogous to `alpha_heatmap` but applied to face.");
// OpenPose Rendering Hand
DEFINE_double(hand_render_threshold, 0.2, "Analogous to `render_threshold`, but applied to the hand keypoints.");
DEFINE_int32(hand_render, -1, "Analogous to `render_pose` but applied to the hand. Extra option: -1 to use the same"
	" configuration that `render_pose` is using.");
DEFINE_double(hand_alpha_pose, 0.6, "Analogous to `alpha_pose` but applied to hand.");
DEFINE_double(hand_alpha_heatmap, 0.7, "Analogous to `alpha_heatmap` but applied to hand.");
// Display
DEFINE_bool(fullscreen, false, "Run in full-screen mode (press f during runtime to toggle).");
DEFINE_bool(process_real_time, false, "Enable to keep the original source frame rate (e.g. for video). If the processing time is"
	" too long, it will skip frames. If it is too fast, it will slow it down.");
DEFINE_bool(no_gui_verbose, false, "Do not write text on output images on GUI (e.g. number of current frame and people). It"
	" does not affect the pose rendering.");
DEFINE_bool(no_display, false, "Do not open a display window.");
// Result Saving
DEFINE_string(write_images, "", "Directory to write rendered frames in `write_images_format` image format.");
DEFINE_string(write_images_format, "png", "File extension and format for `write_images`, e.g. png, jpg or bmp. Check the OpenCV"
	" function cv::imwrite for all compatible extensions.");
DEFINE_string(write_video, "", "Full file path to write rendered frames in motion JPEG video format. It might fail if the"
	" final path does not finish in `.avi`. It internally uses cv::VideoWriter.");
DEFINE_string(write_keypoint, "", "Directory to write the people body pose keypoint data. Set format with `write_keypoint_format`.");
DEFINE_string(write_keypoint_format, "yml", "File extension and format for `write_keypoint`: json, xml, yaml & yml. Json not available"
	" for OpenCV < 3.0, use `write_keypoint_json` instead.");
DEFINE_string(write_keypoint_json, "", "Directory to write people pose data in *.json format, compatible with any OpenCV version.");
DEFINE_string(write_coco_json, "", "Full file path to write people pose data with *.json COCO validation format.");
DEFINE_string(write_heatmaps, "", "Directory to write heatmaps in *.png format. At least 1 `add_heatmaps_X` flag must be"
	" enabled.");
DEFINE_string(write_heatmaps_format, "png", "File extension and format for `write_heatmaps`, analogous to `write_images_format`."
	" Recommended `png` or any compressed and lossless format.");


DEFINE_bool(face_training_features, false, "Write file with training features");
DEFINE_string(write_face_features_file, "./face_training_features.csv", "Full file path to write csv of face posekeypoints and angles");
DEFINE_string(write_image_prefix, "img", "Image prefix name");
DEFINE_string(write_images_path, "./output", "Directory to write images");
DEFINE_bool(mqtt, true, "connect to mqtt server");
DEFINE_bool(show_audience, false, "show audience");
DEFINE_string(face_classifier, "C:/Users/CTI/Documents/RAP_OpenposeGRPC/face_model.pkl", "Classifier model to predict face orientation");
DEFINE_string(face_features_scaler, "C:/Users/CTI/Documents/RAP_OpenposeGRPC/face_scaler.pkl", "Feature scaler for features used in face orientation prediction");
DEFINE_bool(record_video, false, "records video");
DEFINE_bool(no_streaming_input, false, "start server to receive images via grpc streaming");