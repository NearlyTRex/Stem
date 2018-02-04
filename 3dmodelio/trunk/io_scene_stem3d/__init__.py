import bpy
import bmesh
import os
import base64
import mathutils
import mathutils.geometry
import struct
import re
import subprocess

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

def find_material_name(blender_mesh):
	material_name = None
	for blender_object in bpy.data.objects:
		if blender_object.type == 'MESH' and blender_object.data == blender_mesh:
			material_name = blender_object.active_material.name
			break
	return material_name

def write_mesh(context, file_path, blender_mesh, binary_format):
	vertices, indexes = get_primitives(blender_mesh)
	file = open(file_path, "wb")
	file.write(b"{\n\t\"format_version\": 0,\n\t\"format_type\": \"mesh\",\n\t\"name\": \"")
	file.write(escape_string(blender_mesh.name).encode())
	file.write(b"\",\n\t\"armature\": null,\n\t\"material\": ")
	material_name = find_material_name(blender_mesh)
	if material_name is not None:
		file.write(b"\"" + material_name.encode() + b".material\"")
	else:
		file.write(b"null")
	file.write(b",\n\t\"vertices\": \"")
	file.write(encode_vertices(vertices))
	file.write(b"\",\n\t\"indexes\": \"")
	file.write(encode_indexes(indexes))
	file.write(b"\"\n}")
	file.close()

def write_image(context, file_path, blender_image):
	context.scene.render.image_settings.file_format = 'PNG'
	context.scene.render.image_settings.color_depth = '8'
	blender_image.save_render(file_path, context.scene)

def write_material(context, file_path, blender_material, binary_format):
	file = open(file_path, "wb")
	file.write(b"{\n\t\"format_version\": 0,\n\t\"format_type\": \"material\",\n\t\"name\": \"")
	file.write(escape_string(blender_material.name).encode())
	file.write(b"\",\n\t\"color\": {\"red\": ")
	file.write(str(blender_material.diffuse_color[0] * blender_material.diffuse_intensity).encode())
	file.write(b", \"green\": ")
	file.write(str(blender_material.diffuse_color[1] * blender_material.diffuse_intensity).encode())
	file.write(b", \"blue\": ")
	file.write(str(blender_material.diffuse_color[2] * blender_material.diffuse_intensity).encode())
	file.write(b", \"alpha\": ")
	if blender_material.use_transparency:
		file.write(str(blender_material.alpha).encode())
	else:
		file.write(b"1.0")
	file.write(b"},\n\t\"specularity\": ")
	file.write(str(blender_material.specular_intensity).encode())
	file.write(b",\n\t\"shininess\": ")
	file.write(str(128.0 * (float(blender_material.specular_hardness) - 1.0) / 510.0).encode())
	file.write(b",\n\t\"emissiveness\": ")
	file.write(str(blender_material.emit).encode())
	file.write(b",\n\t\"textures\": {\n\t\t\"color_map\": {\n\t\t\t\"name\": ")
	
	color_map_name = None
	normal_map_name = None
	bump_map_name = None
	bump_strength = 0.0
	for blender_texture_slot in blender_material.texture_slots:
		if blender_texture_slot and blender_texture_slot.texture and blender_texture_slot.texture.type == 'IMAGE' and blender_texture_slot.texture.image is not None:
			texture_name = blender_texture_slot.texture.image.name + ".png"
			write_image(context, os.path.join(os.path.dirname(file_path), texture_name), blender_texture_slot.texture.image)
			
			if blender_texture_slot.use_map_color_diffuse:
				color_map_name = texture_name
			if blender_texture_slot.use_map_normal:
				if blender_texture_slot.texture.use_normal_map:
					normal_map_name = texture_name
				else:
					bump_map_name = texture_name
					bump_strength = blender_texture_slot.normal_factor
	
	if color_map_name is not None:
		file.write(b"\"" + color_map_name.encode() + b"\"")
	else:
		file.write(b"null")
	file.write(b",\n\t\t\t\"magnify_nearest\": false\n\t\t},\n\t\t\"normal_map\": {\n\t\t\t\"name\": ")
	if normal_map_name is not None:
		file.write(b"\"" + normal_map_name.encode() + b"\"")
	else:
		file.write(b"null")
	file.write(b",\n\t\t\t\"magnify_nearest\": false\n\t\t}\n\t}\n}")
	file.close()
	
	if bump_map_name is not None and normal_map_name is not None:
		normal_map_path = os.path.join(os.path.dirname(file_path), normal_map_name)
		bump_normal_map_path = os.path.join(os.path.dirname(file_path), os.path.splitext(bump_map_name)[0] + "_converted.png")
		bump_map_path = os.path.join(os.path.dirname(file_path), bump_map_name)
		bump2normal_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "bump2normal")
		subprocess.run([bump2normal_path, bump_map_path, bump_normal_map_path, str(bump_strength * 32)])
		subprocess.run([bump2normal_path, "--merge", bump_normal_map_path, normal_map_path, normal_map_path])
		os.unlink(bump_normal_map_path)
		os.unlink(bump_map_path)

class ExportStem3D(bpy.types.Operator, ExportHelper):
	bl_idname = 'export_scene.stem3d'
	bl_label = 'Export Stem3D'
	
	filename_ext = '.stem3d'
	filter_glob = StringProperty(default='*.stem3d', options={'HIDDEN'})
	
	export_format = 'ASCII'
	
	def execute(self, context):
		try:
			os.mkdir(self.filepath)
		except FileExistsError:
			pass
		
		#file_path = bpy.path.ensure_ext(self.filepath, self.filename_ext)
		for blender_mesh in bpy.data.meshes:
			blender_mesh_copy = blender_mesh.copy()
			triangulated_mesh = bmesh.new()
			triangulated_mesh.from_mesh(blender_mesh_copy)
			bmesh.ops.triangulate(triangulated_mesh, faces=triangulated_mesh.faces[:], quad_method=0, ngon_method=0)
			triangulated_mesh.to_mesh(blender_mesh_copy)
			triangulated_mesh.free()
			write_mesh(context, os.path.join(self.filepath, bpy.path.ensure_ext(blender_mesh.name, ".mesh")), blender_mesh_copy, False)
		for blender_material in bpy.data.materials:
			write_material(context, os.path.join(self.filepath, bpy.path.ensure_ext(blender_material.name, ".material")), blender_material, False)
		#for blender_armature in bpy.data.armatures:
		#	write_armature(self.filepath, blender_armature)
		#for blender_texture in bpy.data.textures:
		#	write_texture(self.filepath, blender_texture)
		return {'FINISHED'}

def register():
	bpy.utils.register_class(ExportStem3D)
	bpy.types.INFO_MT_file_export.append(menu_func_export_stem3d)

def unregister():
	bpy.utils.unregister_class(ExportStem3D)
	bpy.types.INFO_MT_file_export.remove(menu_func_export_stem3d)

def menu_func_export_stem3d(self, context):
	self.layout.operator(ExportStem3D.bl_idname, text='stem3d (JSON)')
