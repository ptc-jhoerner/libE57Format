#ifndef E57SIMPLEIMPL_H_INCLUDED
#define E57SIMPLEIMPL_H_INCLUDED
//////////////////////////////////////////////////////////////////////////
//
//  E57SimpleImpl.h - private implementation header of E57 format reference implementation.
//
//	Copyright (c) 2010 Stan Coleby (scoleby@intelisum.com)
//	All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person or organization
//  obtaining a copy of the software and accompanying documentation covered by
//  this license (the "Software") to use, reproduce, display, distribute,
//  execute, and transmit the Software, and to prepare derivative works of the
//  Software, and to permit third-parties to whom the Software is furnished to
//  do so, all subject to the following:
// 
//  The copyright notices in the Software and this entire statement, including
//  the above license grant, this restriction and the following disclaimer,
//  must be included in all copies of the Software, in whole or in part, and
//  all derivative works of the Software, unless such copies or derivative
//  works are solely in the form of machine-executable object code generated by
//  a source language processor.
// 
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
//  FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//
//	The Boost license Vestion 1.0 - August 17th, 2003 is discussed in
//	http://www.boost.org/users/license.html.
//
//  This source code is only intended as a supplement to promote the
//	ASTM E57.04 3D Imaging System File Format standard for interoperability
//	of Lidar Data.  See http://www.libe57.org.
//
//////////////////////////////////////////////////////////////////////////
//
//	New E57SimpleImpl.h
//	V1	May 18, 2010	Stan Coleby		scoleby@intelisum.com
//  V6	June 8, 2010	Stan Coleby		scoleby@intelisum.com
//
//////////////////////////////////////////////////////////////////////////

//! @file E57SimpleImpl.h

#include "E57Simple.h"

#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdint.h>

// Define the following symbol adds some functions to the API for implementation purposes.
// These functions are not available to a normal API user.
#define E57_INTERNAL_IMPLEMENTATION_ENABLE 1

namespace e57 {

std::string	GetNewGuid();

////////////////////////////////////////////////////////////////////
//
//	e57::ReaderImpl
//

//! This is the E57 Reader class

class	ReaderImpl {

private:

	ImageFile		imf_;
	StructureNode	root_;

	VectorNode		data3D_;

	VectorNode		images2D_;

public:

//! This function is the constructor for the reader class
					ReaderImpl(
						const ustring & filePath		//!< file path string
						);

//! This function is the destructor for the reader class
virtual				~ReaderImpl();

//! This function returns true if the file is open
virtual	bool		IsOpen();

//! This function closes the file
virtual	bool		Close();

////////////////////////////////////////////////////////////////////
//
//	File information
//
//! This function returns the file header information
virtual bool		GetE57Root(
						E57Root & fileHeader	//!< This is the main header information
					    );	//!< /return Returns true if sucessful

////////////////////////////////////////////////////////////////////
//
//	Camera Image picture data
//
//! This function returns the total number of Picture Blocks
virtual	int64_t		GetImage2DCount( void);

//! This function returns the Image2Ds header and positions the cursor
virtual bool		ReadImage2D( 
						int64_t				imageIndex,		//!< This in the index into the Image2Ds vector
						Image2D &			Image2DHeader	//!< pointer to the Image2D structure to receive the picture information
						);						//!< /return Returns true if sucessful

//! This function returns the size of the image data
virtual bool		GetImage2DSizes(
						int64_t					imageIndex,		//!< This in the index into the image2D vector
						e57::Image2DProjection&	imageProjection,//!< identifies the projection desired.
						e57::Image2DType &		imageType,		//!< identifies the image format desired.
						int64_t &				imageWidth,		//!< The image width (in pixels).
						int64_t &				imageHeight,	//!< The image height (in pixels).
						int64_t &				imageSize,		//!< This is the total number of bytes for the image blob.
						e57::Image2DType &		imageMaskType,	//!< This is E57_PNG_IMAGE_MASK if "imageMask" is defined in the projection
						e57::Image2DType &		imageVisualType	//!< This is image type of the VisualReferenceRepresentation if given.
						);										//!< /return Returns true if sucessful

//! This function reads the block
virtual	int64_t		ReadImage2DData(
						int64_t					imageIndex,		//!< picture block index
						e57::Image2DProjection	imageProjection,//!< identifies the projection desired.
						e57::Image2DType		imageType,		//!< identifies the image format desired.
						void *					pBuffer,		//!< pointer the buffer
						int64_t					start,			//!< position in the block to start reading
						int64_t					count			//!< size of desired chuck or buffer size
						);										//!< /return Returns the number of bytes transferred.

//! This function reads one of the image blobs
virtual bool		GetImage2DNodeSizes(
						e57::StructureNode		image,			//!< 1 of 3 projects or the visual
						e57::Image2DType &		imageType,		//!< identifies the image format desired.
						int64_t &				imageWidth,		//!< The image width (in pixels).
						int64_t &				imageHeight,	//!< The image height (in pixels).
						int64_t &				imageSize,		//!< This is the total number of bytes for the image blob.
						e57::Image2DType &		imageMaskType	//!< This is E57_PNG_IMAGE_MASK if "imageMask" is defined in the projection
						);										//!< /return Returns true if sucessful

virtual int64_t		ReadImage2DNode(
						e57::StructureNode		image,			//!< 1 of 3 projects or the visual
						e57::Image2DType		imageType,		//!< identifies the image format desired.
						void *					pBuffer,		//!< pointer the buffer
						int64_t					start,			//!< position in the block to start reading
						int64_t					count			//!< size of desired chuck or buffer size
						);

////////////////////////////////////////////////////////////////////
//
//	Scanner Image 3d data
//
//! This function returns the total number of Image Blocks
virtual	int64_t		GetData3DCount( void);

//! This function returns the Data3D header and positions the cursor
virtual bool		ReadData3D( 
						int64_t		dataIndex,	//!< This in the index into the images3D vector
						Data3D &	data3DHeader //!< pointer to the Data3D structure to receive the image information
						);	//!< /return Returns true if sucessful

//! This function returns the size of the point data
virtual	bool		GetData3DSizes(
						int64_t		dataIndex,	//!< This in the index into the images3D vector
						int64_t &	rowMax,		//!< This is the maximum row size
						int64_t &	columnMax,	//!< This is the maximum column size
						int64_t &	pointsSize,	//!< This is the total number of point records
						int64_t &	groupsSize,	//!< This is the total number of group reocrds
						int64_t &	countSize,	//!< This is the maximum point count per group
						bool &		bColumnIndex	//!< This indicates that the idElementName is "columnIndex"
						);

//! This funtion writes out the group data
virtual bool		ReadData3DGroupsData(
						int64_t		dataIndex,			//!< data block index given by the NewData3D
						int64_t		groupCount,			//!< size of each of the buffers given
						int64_t*	idElementValue,		//!< index for this group
						int64_t*	startPointIndex,	//!< Starting index in to the "points" data vector for the groups
						int64_t*	pointCount			//!< size of the groups given
						);								//!< \return Return true if sucessful, false otherwise

//! This function sets up the point data fields 
/* All the non-NULL buffers in the call below have number of elements = pointCount.
Call the CompressedVectorReader::read() until all data is read.
*/

virtual CompressedVectorReader	SetUpData3DPointsData(
						int64_t		dataIndex,			//!< data block index given by the NewData3D
						size_t		pointCount,			//!< size of each element buffer.
						const Data3DPointsData& buffers //!< pointers to user-provided buffers
						);

//! This function returns the file raw E57Root Structure Node
virtual	StructureNode		GetRawE57Root();	//!< /return Returns the E57Root StructureNode
//! This function returns the raw Data3D Vector Node
virtual VectorNode			GetRawData3D();		//!< /return Returns the raw Data3D VectorNode
//! This function returns the raw Image2D Vector Node
virtual VectorNode			GetRawImages2D();	//!< /return Returns the raw Image2D VectorNode
//! This function returns the ram ImageFile Node which is need to add enhancements
virtual ImageFile			GetRawIMF();  //!< /return Returns the raw ImageFile
}; //end Reader class


////////////////////////////////////////////////////////////////////
//
//	e57::Writer
//

//! This is the E57 Writer class

class	WriterImpl {

private:
	ImageFile				imf_;
	StructureNode			root_;

	VectorNode				data3D_;

	VectorNode				images2D_;

public:

//! This function is the constructor for the writer class
					WriterImpl(
						const ustring & filePath,		//!< file path string
						const ustring & coordinateMetaData	//!< Information describing the Coordinate Reference System to be used for the file
						);

//! This function is the destructor for the writer class
virtual				~WriterImpl();

//! This function returns true if the file is open
virtual	bool		IsOpen();

//! This function closes the file
virtual	bool		Close();

////////////////////////////////////////////////////////////////////
//
//	Camera Image picture data
//

//! This function sets up the Image2Ds header and positions the cursor
//* The user needs to config a Image2D structure with all the camera information before making this call. */
virtual int64_t		NewImage2D(
						Image2D &	image2DHeader	//!< pointer to the Image2D structure to receive the picture information
						);						//!< /return Returns the Image2D index

//! This function writes the block
virtual	int64_t		WriteImage2DData(
						int64_t					imageIndex,		//!< picture block index given by the NewImage2D
						e57::Image2DType		imageType,		//!< identifies the image format desired.
						e57::Image2DProjection	imageProjection,//!< identifies the projection desired.
						void *					pBuffer,		//!< pointer the buffer
						int64_t					start,			//!< position in the block to start writing
						int64_t					count			//!< size of desired chuck or buffer size
						);										//!< /return Returns the number of bytes written

//! This function writes the projection image
virtual int64_t		WriteImage2DNode(
						e57::StructureNode		image,			//!< 1 of 3 projects or the visual
						e57::Image2DType		imageType,		//!< identifies the image format desired.
						void *					pBuffer,		//!< pointer the buffer
						int64_t					start,			//!< position in the block to start reading
						int64_t					count			//!< size of desired chuck or buffer size
						);

//! This function sets up the Data3D header and positions the cursor for the binary data
//* The user needs to config a Data3D structure with all the scanning information before making this call. */

virtual int64_t		NewData3D(
						Data3D &	data3DHeader,	//!< pointer to the Data3D structure to receive the image information
						bool		(*pointExtension)(ImageFile	imf,StructureNode proto)	//!< function pointer to add point data extension
						);							//!< /return Returns the index of the new scan's data3D block.

//! This function writes out blocks of point data
virtual CompressedVectorWriter	SetUpData3DPointsData(
						int64_t		dataIndex,			//!< data block index given by the NewData3D
						size_t		pointCount,			//!< size of each of the buffers given
						const Data3DPointsData& buffers //!< pointers to user-provided buffers
						);


//! This funtion writes out the group data
virtual bool		WriteData3DGroupsData(
						int64_t		dataIndex,			//!< data block index given by the NewData3D
						int64_t		groupCount,			//!< size of each of the buffers given
						int64_t*	idElementValue,		//!< index for this group
						int64_t*	startPointIndex,	//!< Starting index in to the "points" data vector for the groups
						int64_t*	pointCount			//!< size of the groups given
						);								//!< \return Return true if sucessful, false otherwise

//! This function returns the file raw E57Root Structure Node
virtual	StructureNode		GetRawE57Root();	//!< /return Returns the E57Root StructureNode
//! This function returns the raw Data3D Vector Node
virtual VectorNode			GetRawData3D();		//!< /return Returns the raw Data3D VectorNode
//! This function returns the raw Image2D Vector Node
virtual VectorNode			GetRawImages2D();	//!< /return Returns the raw Image2D VectorNode
//! This function returns the ram ImageFile Node which is need to add enhancements
virtual ImageFile			GetRawIMF();  //!< /return Returns the raw ImageFile

}; //end Writer class

}; //end namespace

#endif // E57SIMPLEIMPL_H_INCLUDED
