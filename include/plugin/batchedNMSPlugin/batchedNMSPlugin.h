/*
 * Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TRT_BATCHED_NMS_PLUGIN_H
#define TRT_BATCHED_NMS_PLUGIN_H
#include <string>
#include <vector>

#include "NvInferPlugin.h"

using namespace nvinfer1::plugin;
namespace nvinfer1 {
namespace plugin {

class BatchedNMSPlugin : public IPluginV2DynamicExt {
 public:
  BatchedNMSPlugin(NMSParameters param);

  BatchedNMSPlugin(const void* data, size_t length);

  ~BatchedNMSPlugin() override = default;

  int getNbOutputs() const override;

  nvinfer1::DimsExprs getOutputDimensions(
      int outputIndex, const nvinfer1::DimsExprs* inputs, int nbInputs,
      nvinfer1::IExprBuilder& exprBuilder) override;

  int initialize() override;

  void terminate() override;

  size_t getWorkspaceSize(const nvinfer1::PluginTensorDesc* inputs,
                          int nbInputs,
                          const nvinfer1::PluginTensorDesc* outputs,
                          int nbOutputs) const override;

  int enqueue(const nvinfer1::PluginTensorDesc* inputDesc,
              const nvinfer1::PluginTensorDesc* outputDesc,
              const void* const* inputs, void* const* outputs, void* workSpace,
              cudaStream_t stream) override;

  size_t getSerializationSize() const override;

  void serialize(void* buffer) const override;

  void configurePlugin(const nvinfer1::DynamicPluginTensorDesc* inputs,
                       int nbInputs,
                       const nvinfer1::DynamicPluginTensorDesc* outputs,
                       int nbOutputs) override;

  bool supportsFormatCombination(int pos,
                                 const nvinfer1::PluginTensorDesc* inOut,
                                 int nbInputs, int nbOutputs) override;

  const char* getPluginType() const override;

  const char* getPluginVersion() const override;

  void destroy() override;

  IPluginV2DynamicExt* clone() const override;

  nvinfer1::DataType getOutputDataType(int index,
                                       const nvinfer1::DataType* inputType,
                                       int nbInputs) const override;

  void setPluginNamespace(const char* libNamespace) override;

  const char* getPluginNamespace() const override;

  void setClipParam(bool clip);

 private:
  NMSParameters param{};
  int boxesSize{};
  int scoresSize{};
  int numPriors{};
  std::string mNamespace;
  bool mClipBoxes{};

 protected:
  // To prevent compiler warnings.
  using nvinfer1::IPluginV2DynamicExt::canBroadcastInputAcrossBatch;
  using nvinfer1::IPluginV2DynamicExt::configurePlugin;
  using nvinfer1::IPluginV2DynamicExt::enqueue;
  using nvinfer1::IPluginV2DynamicExt::getOutputDimensions;
  using nvinfer1::IPluginV2DynamicExt::getWorkspaceSize;
  using nvinfer1::IPluginV2DynamicExt::isOutputBroadcastAcrossBatch;
  using nvinfer1::IPluginV2DynamicExt::supportsFormat;
};

class BatchedNMSPluginCreator : public IPluginCreator {
 public:
  BatchedNMSPluginCreator();

  ~BatchedNMSPluginCreator() override = default;

  const char* getPluginName() const override;

  const char* getPluginVersion() const override;

  const PluginFieldCollection* getFieldNames() override;

  IPluginV2Ext* createPlugin(const char* name,
                             const PluginFieldCollection* fc) override;

  IPluginV2Ext* deserializePlugin(const char* name, const void* serialData,
                                  size_t serialLength) override;

  void setPluginNamespace(const char* libNamespace) override;

  const char* getPluginNamespace() const override;

 private:
  static PluginFieldCollection mFC;
  NMSParameters params;
  static std::vector<PluginField> mPluginAttributes;
  bool mClipBoxes;
  std::string mNamespace;
};
REGISTER_TENSORRT_PLUGIN(BatchedNMSPluginCreator);
}  // namespace plugin
}  // namespace nvinfer1

#endif  // TRT_BATCHED_NMS_PLUGIN_H
