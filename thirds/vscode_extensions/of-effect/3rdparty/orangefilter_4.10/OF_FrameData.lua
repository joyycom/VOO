--* This Document is AutoGenerate by OrangeFilter, Don't Change it! *

---@meta

---
---[3.2]frame data[parent:]
---
---@class OF_FrameData
---
---[3.2]camera matrix
---
---@field cameraMat float[]
---
---[3.2]timestamp of this frame
---
---@field timestamp float
---
---[3.2]has track camera matrix data in cameraMat,1 or 0
---
---@field trackOn int
---
---[3.2]pick point
---
---@field pickPoint float[]
---
---[3.2]is pick on, 1 or 0
---
---@field pickOn int
---
---[3.2]face data array
---
---@field faceFrameDataArr OF_FaceFrameDataArr#of_faceframedataarr
---
---[3.2]hand gesture data array
---
---@field gestureDataArr OF_GestureFrameDataArr#of_gestureframedataarr
---
---[3.2]body data array
---
---@field bodyFrameDataArr OF_BodyFrameDataArr#of_bodyframedataarr
---
---[4.3]body 3d data array
---
---@field body3DFrameDataArr OF_Body3DFrameDataArr#of_body3dframedataarr
---
---[3.2]segment data array
---
---@field segmentFrameData OF_SegmentFrameData#of_segmentframedata
---
---[3.2]audio data array
---
---@field audioFrameData OF_AudioFrameData#of_audioframedata
---
---[4.9]ext data
---
---@field extData OF_Variant#of_variant
---
---[3.3]ar camera data
---
---@field arCameraData OF_ArCameraData#of_arcameradata

OF_FrameData = {}

---
---[3.2]constructor, no param
---
--- @nodiscard
function OF_FrameData:new() end

---
---[4.5]update projection matrix
---
--- @param arg0 OF_FrameData#of_framedata
--- @param arg1 Matrix4f#matrix4f
--- @nodiscard
function OF_FrameData.UpdateProjectMatrix(arg0, arg1) end
---
---[3.2]clone source data,return a new one
---
--- @param arg0 OF_FrameData#of_framedata
--- @return OF_FrameData#of_framedata
--- @nodiscard
function OF_FrameData.CloneFrameData(arg0) end
---
---[3.2]copy data from source to dest
---
--- @param arg0 OF_FrameData#of_framedata
--- @param arg1 OF_FrameData#of_framedata
--- @nodiscard
function OF_FrameData.CopyFrameData(arg0, arg1) end
---
---[3.2]destroy frame data
---
--- @param arg0 OF_FrameData#of_framedata
--- @nodiscard
function OF_FrameData.DestroyFrameData(arg0) end

return OF_FrameData
