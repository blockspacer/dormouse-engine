#ifndef _DORMOUSEENGINE_GRAPHICS_SHADERDATATYPE_HPP_
#define _DORMOUSEENGINE_GRAPHICS_SHADERDATATYPE_HPP_

#include <d3dcommon.h>
#include "dormouse-engine/system/windows/cleanup-macros.hpp"

#include "dormouse-engine/enums.hpp"

namespace dormouse_engine::graphics {

struct ShaderDataType {

	DE_MEMBER_ENUM_VALUES(
		Class,
		(SCALAR)(D3D_SVC_SCALAR)
		(VECTOR)(D3D_SVC_VECTOR)
		(MATRIX_ROW_MAJOR)(D3D_SVC_MATRIX_ROWS)
		(MATRIX_COLUMN_MAJOR)(D3D_SVC_MATRIX_COLUMNS)
		(OBJECT)(D3D_SVC_OBJECT)
		(STRUCT)(D3D_SVC_STRUCT)
	);

	DE_MEMBER_ENUM_VALUES(
		ScalarType,
		(EMPTY)(D3D_SVT_VOID)
		(BOOL)(D3D_SVT_BOOL)
		(INT)(D3D_SVT_INT)
		(UINT)(D3D_SVT_UINT)
		(FLOAT)(D3D_SVT_FLOAT)
	);

	ShaderDataType() = default;

	ShaderDataType(Class klass, ScalarType scalarType, size_t columns, size_t rows) :
		klass(klass),
		scalarType(scalarType),
		columns(columns),
		rows(rows)
	{
	}

	Class klass;
	ScalarType scalarType;
	size_t columns;
	size_t rows;
};

} // namespace dormouse_engine::graphics

#endif /* _DORMOUSEENGINE_GRAPHICS_SHADERDATATYPE_HPP_ */
