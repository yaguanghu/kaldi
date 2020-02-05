// pybind/feat/online_feature_pybind.cc

// Copyright 2019   Microsoft Corporation (author: Xingyu Na)

// See ../../../COPYING for clarification regarding multiple authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED
// WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
// MERCHANTABLITY OR NON-INFRINGEMENT.
// See the Apache 2 License for the specific language governing permissions and
// limitations under the License.

#include "feat/online_feature_pybind.h"

#include "feat/feature-mfcc.h"
#include "feat/feature-fbank.h"
#include "feat/online-feature.h"

using namespace kaldi;
  
template <class Feature>
void online_base_feature(py::module& m, const std::string& feat_type) {
  py::class_<OnlineGenericBaseFeature<Feature>, OnlineBaseFeature>(m, feat_type.c_str())
      .def(py::init<const typename Feature::Options&>(), py::arg("opts"));
}

void pybind_online_feature(py::module& m) {
  py::class_<OnlineFeatureInterface>(m, "OnlineFeatureInterface",
      "OnlineFeatureInterface is an interface for online feature processing."
      "This interface only specifies how the object *outputs* the features."
      "How it obtains the features, e.g. from a previous object or objects of type"
      "OnlineFeatureInterface, is not specified in the interface and you will"
      "likely define new constructors or methods in the derived type to do that.")
      .def("Dim", &OnlineFeatureInterface::Dim)
      .def("NumFramesReady", &OnlineFeatureInterface::NumFramesReady)
      .def("IsLastFrame", &OnlineFeatureInterface::IsLastFrame)
      .def("GetFrame", &OnlineFeatureInterface::GetFrame)
      .def("GetFrames", &OnlineFeatureInterface::GetFrames)
      .def("FrameShiftInSeconds", &OnlineFeatureInterface::FrameShiftInSeconds);

  py::class_<OnlineBaseFeature, OnlineFeatureInterface>(m, "OnlineBaseFeature")
      .def("AcceptWaveform", &OnlineBaseFeature::AcceptWaveform,
           "This would be called from the application, when you get more wave data."
           "Note: the sampling_rate is typically only provided so the code can assert"
           "that it matches the sampling rate expected in the options.")
      .def("InputFinished", &OnlineBaseFeature::InputFinished,
           "InputFinished() tells the class you won't be providing any"
           "more waveform.  This will help flush out the last few frames"
           "of delta or LDA features (it will typically affect the return value"
           "of IsLastFrame.");

  online_base_feature<MfccComputer>(m, "OnlineMfcc");
  online_base_feature<FbankComputer>(m, "OnlineFbank");

  py::class_<OnlineCmvnOptions>(m, "OnlineCmvnOptions")
      .def(py::init<>())
      .def_readwrite("cmn_window", &OnlineCmvnOptions::cmn_window)
      .def_readwrite("speaker_frames", &OnlineCmvnOptions::speaker_frames)
      .def_readwrite("global_frames", &OnlineCmvnOptions::global_frames)
      .def_readwrite("normalize_mean", &OnlineCmvnOptions::normalize_mean)
      .def_readwrite("normalize_variance", &OnlineCmvnOptions::normalize_variance)
      .def_readwrite("modulus", &OnlineCmvnOptions::modulus)
      .def_readwrite("ring_buffer_size", &OnlineCmvnOptions::ring_buffer_size)
      .def_readwrite("skip_dims", &OnlineCmvnOptions::skip_dims)
      .def("Check", &OnlineCmvnOptions::Check);

  py::class_<OnlineCmvnState>(m, "OnlineCmvnState",
      "This bind is only used internally, so members are not exposed.")
      .def(py::init<>())
      .def(py::init<const Matrix<double>&>())
      .def(py::init<const OnlineCmvnState&>());

  py::class_<OnlineCmvn, OnlineFeatureInterface>(m, "OnlineCmvn")
      .def(py::init<const OnlineCmvnOptions&, OnlineFeatureInterface*>(),
           py::arg("opts"), py::arg("src"))
      .def(py::init<const OnlineCmvnOptions&, const OnlineCmvnState&,
           OnlineFeatureInterface*>(),
           py::arg("opts"), py::arg("stat"), py::arg("src"))
      .def("GetState", &OnlineCmvn::GetState)
      .def("SetState", &OnlineCmvn::SetState)
      .def("Freeze", &OnlineCmvn::Freeze);

  py::class_<OnlineSpliceOptions>(m, "OnlineSpliceOptions")
      .def(py::init<>())
      .def_readwrite("left_context", &OnlineSpliceOptions::left_context)
      .def_readwrite("right_context", &OnlineSpliceOptions::right_context);

  py::class_<OnlineSpliceFrames, OnlineFeatureInterface>(m, "OnlineSpliceFrames")
      .def(py::init<const OnlineSpliceOptions&, OnlineFeatureInterface*>(),
           py::arg("opts"), py::arg("src"));

  py::class_<OnlineTransform, OnlineFeatureInterface>(m, "OnlineTransform")
      .def(py::init<const Matrix<float>&, OnlineFeatureInterface*>());

  py::class_<OnlineCacheFeature, OnlineFeatureInterface>(m, "OnlineCacheFeature")
      .def(py::init<OnlineFeatureInterface*>(), py::arg("src"))
      .def("ClearCache", &OnlineCacheFeature::ClearCache);

  py::class_<OnlineAppendFeature, OnlineFeatureInterface>(m, "OnlineAppendFeature")
      .def(py::init<OnlineFeatureInterface*, OnlineFeatureInterface*>(),
           py::arg("src1"), py::arg("src2"));

}
