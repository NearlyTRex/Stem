import bpy
import os
import base64
import mathutils
import mathutils.geometry
import struct
import re

from bpy.props import (StringProperty)
from bpy_extras.io_utils import (ExportHelper)

bl_info = {
	"name": "Stem3D exporter",
	"author": "Alex Diener",
	"blender": (2, 78, 0),
	"location": "File > Export",
	"description": "Export as Stem3D",
	"category": "Import-Export"
}

def escape_string(string):
	return re.sub(r'"', r'\"', string)

def swizzle_position(vector):
	return mathutils.Vector((vector[0], vector[2], -vector[1]))

def swizzle_tangent(vector):
	return mathutils.Vector((vector[0], vector[2], -vector[1], 1.0))

def get_primitives(blender_mesh):
	vertices = []
	indexes = []
	
	blender_mesh.calc_tangents()
	index = 0
	for blender_polygon in blender_mesh.polygons:
		loop_index_list = []
		
		if len(blender_polygon.loop_indices) == 3:
			loop_index_list.extend(blender_polygon.loop_indices)
			
		elif len(blender_polygon.loop_indices) > 3:
			polyline = []
			for loop_index in blender_polygon.loop_indices:
				vertex_index = blender_mesh.loops[loop_index].vertex_index
				vertex = blender_mesh.vertices[vertex_index].co
				polyline.append(mathutils.Vector((vertex[0], vertex[1], vertex[2])))
			
			triangles = mathutils.geometry.tessellate_polygon((polyline,))
			
			for triangle in triangles:
				loop_index_list.append(blender_polygon.loop_indices[triangle[0]])
				loop_index_list.append(blender_polygon.loop_indices[triangle[2]])
				loop_index_list.append(blender_polygon.loop_indices[triangle[1]])
			
		else:
			continue
		
		face_normal = blender_polygon.normal
		face_tangent = mathutils.Vector((0.0, 0.0, 0.0))
		for loop_index in blender_polygon.loop_indices:
			vertex = blender_mesh.loops[loop_index]
			face_tangent += vertex.tangent
		face_tangent.normalize()
		
		flip_tangent = False
		for loop_index in blender_polygon.loop_indices:
			vertex_index = blender_mesh.loops[loop_index].vertex_index
			vertex = blender_mesh.vertices[vertex_index].co
			if vertex[0] < 0.0:
				flip_tangent = True
				break
		
		for loop_index in loop_index_list:
			vertex_index = blender_mesh.loops[loop_index].vertex_index
			vertex = blender_mesh.vertices[vertex_index]
			position = None
			normal = None
			tangent = None
			tex_coord = None
			color = None
			bones = None
			weights = None
			position = swizzle_position(vertex.co)
			if blender_polygon.use_smooth:
				normal = swizzle_position(vertex.normal)
				tangent = swizzle_tangent(blender_mesh.loops[loop_index].tangent)
			else:
				normal = swizzle_position(face_normal)
				tangent = swizzle_tangent(face_tangent)
			if flip_tangent:
				tangent[3] = -tangent[3]
			if blender_mesh.uv_layers.active:
				tex_coord = blender_mesh.uv_layers.active.data[loop_index].uv
			else:
				tex_coord = (0.0, 0.0)
			color = (1.0, 1.0, 1.0, 1.0)
			#bones = (0, 0, 0, 0)
			#weights = (1.0, 0.0, 0.0, 0.0)
			vertices.append([position, tex_coord, normal, tangent, color])
			indexes.append(index)
			index += 1
	
	return vertices, indexes

def encode_vertices(vertices):
	vertices_packed = bytearray()
	vertex_size = 12 * 4
	for vertex in vertices:
		vertices_packed += struct.pack("<fff", vertex[0][0], vertex[0][1], vertex[0][2])
		vertices_packed += struct.pack("<ff", vertex[1][0], vertex[1][1])
		vertices_packed += struct.pack("<fff", vertex[2][0], vertex[2][1], vertex[2][2])
		vertices_packed += struct.pack("<ffff", vertex[3][0], vertex[3][1], vertex[3][2], vertex[3][3])
		vertices_packed += struct.pack("<ffff", vertex[4][0], vertex[4][1], vertex[4][2], vertex[4][3])
	
	encoded_string = base64.encodestring(vertices_packed)
	return encoded_string.replace(b"\n", b"")

def encode_indexes(indexes):
	indexes_packed = bytearray()
	index_size = 4
	for index in indexes:
		indexes_packed += struct.pack("<I", index)
	
	encoded_string = base64.encodestring(indexes_packed)
	return encoded_string.replace(b"\n", b"")

def write_mesh(file_path, blender_mesh):
	vertices, indexes = get_primitives(blender_mesh)
	file = open(file_path, "wb")
	file.write(b"{\n\t\"format_version\": 0,\n\t\"format_type\": \"mesh\",\n\t\"name\": \"")
	file.write(escape_string(blender_mesh.name).encode())
	file.write(b"\",\n\t\"armature\": null,\n\t\"material\": null,\n\t\"vertices\": \"")
	file.write(encode_vertices(vertices))
	file.write(b"\",\n\t\"indexes\": \"")
	file.write(encode_indexes(indexes))
	file.write(b"\"\n}")
	file.close()

class ExportStem3D(bpy.types.Operator, ExportHelper):
	bl_idname = 'export_scene.stem3d'
	bl_label = 'Export Stem3D'
	
	filename_ext = '.stem3d'
	filter_glob = StringProperty(default='*.stem3d', options={'HIDDEN'})
	
	export_format = 'ASCII'
	
	def execute(self, context):
		file_path = bpy.path.ensure_ext(self.filepath, self.filename_ext)
		write_mesh(file_path, bpy.data.meshes[0])
		return {'FINISHED'}

def register():
	bpy.utils.register_class(ExportStem3D)
	bpy.types.INFO_MT_file_export.append(menu_func_export_stem3d)

def unregister():
	bpy.utils.unregister_class(ExportStem3D)
	bpy.types.INFO_MT_file_export.remove(menu_func_export_stem3d)

def menu_func_export_stem3d(self, context):
	self.layout.operator(ExportStem3D.bl_idname, text='stem3d (JSON)')
