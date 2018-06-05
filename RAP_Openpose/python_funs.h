#pragma once
#include <boost\python.hpp>
namespace python = boost::python;
namespace python_funs {

	python::object pythonPredictFun;
	python::object sendImagesFunc;
	python::object sendCSVFunc;

	void initializePythonModule(std::string face_features_scaler, std::string face_classifier) {
		try
		{
			python::object my_python_module = python::import("predictor");
			python::object loadPredictorFun = my_python_module.attr("load_predictor");
			python::object loadResult = loadPredictorFun(face_features_scaler, face_classifier);

			pythonPredictFun = my_python_module.attr("predict");
			sendImagesFunc = my_python_module.attr("send_images");
			sendCSVFunc = my_python_module.attr("send_results");

		}
		catch (const python::error_already_set&)
		{
			PyErr_Print();
		}
	}
}