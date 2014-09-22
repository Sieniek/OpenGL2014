#Memory usage:
#vertices = N * 3 * sizeof(float)
#indices = 2 * (N - N ** 0.5 * 2 - 1) * sizeof(int)
#wall_normals = 2 * (N - N ** 0.5 * 2 - 1) * 3 * sizeof(float)
#vertices_normals = N * 3 * sizeof(float)

from math import sin
from random import random

def mathFunc(x, z):
	return (sin(x/5.0) * sin(z/5.0))

X_RANGE = 10
Y_RANGE = 10
SCALE = 100

vertices = []
indices = []
wall_normals = []
vertices_normals =[]

plik = open('surface.h','w')

#Vertices

plik.write('#include <ode/ode.h>\n\nconst int VertexCount = '+str(X_RANGE * Y_RANGE)+';\nconst int IndexCount = '+str(2 * (X_RANGE - 1)*(Y_RANGE - 1) * 3) +';\n\n')

plik.write('float Vertices[] = {\n')

for x in range(X_RANGE/-2 , X_RANGE/2):
	for z in range(Y_RANGE/-2, Y_RANGE/2):
		y = mathFunc(x,z)
		vertices += [[SCALE * x, SCALE ** 2 * y,SCALE * z]]
		plik.write('\t' + str(SCALE * x) + ',\t' + str(SCALE * y) + ',\t' + str(SCALE * z) + ',\n')

plik.write('};\n')

#Indices

plik.write('dTriIndex Indices[][3] = {\n')

for row in range(X_RANGE - 1):
	for column in range(Y_RANGE - 1):
	
		a_index = row * Y_RANGE + column 
		b_index = row * Y_RANGE + column + 1
		c_index = (row + 1) * Y_RANGE + column
		indices +=[[a_index, b_index, c_index]]
		
		plik.write('\t{' + str(a_index) + ',\t' + str(b_index) + ',\t' + str(c_index) + '},\n')

		a_index = (row + 1) * Y_RANGE + column + 1
		plik.write('\t{' + str(b_index) + ',\t' + str(a_index) + ',\t' + str(c_index) + '},\n')		
		indices +=[[b_index, a_index, c_index]]#this order is important for ODE to compute properly normals
		
plik.write('};\n')

plik.write('float geomTexCoords[] = {\n')
for row in range(X_RANGE):
	for column in range(Y_RANGE):
		if row % 2 == 0 and column % 2 == 0:
			plik.write('\t0,\t0,\n')
		elif row % 2 == 0 and column % 2 == 1:
			plik.write('\t0,\t1.0f,\n')
		elif row % 2 == 1 and column % 2 == 0:
			plik.write('\t1.0f,\t0,\n')
		else:
			plik.write('\t1.0f,\t1.0f, \n')
		
plik.write('};\n')

plik.write('unsigned int gl_Indices[] = {\n')

for row in range(X_RANGE - 1):
	for column in range(Y_RANGE- 1):
		a_index = row * Y_RANGE + column
		b_index = row * Y_RANGE + column + 1
		c_index = (row + 1) * Y_RANGE + column
		
		plik.write('\t' + str(a_index) + ',\t' + str(b_index) + ',\t' + str(c_index) + ',\n')

		a_index = (row + 1) * Y_RANGE + column + 1
		plik.write('\t' + str(a_index) + ',\t' + str(b_index) + ',\t' + str(c_index) + ',\n')		
		
plik.write('};\n')

#Calculate normals vertices (but firstly walls normalls)
for wall in indices:
	a_index = wall[0]
	b_index = wall[1]
	c_index = wall[2]
	
	a = vertices[a_index]
	b = vertices[b_index]
	c = vertices[c_index]
	
	ab = [b[0] - a[0], b[1] - a[1], b[2] - a[2]]
	ac = [c[0] - a[0], c[1] - a[1], c[2] - a[2]]
	
	wall_normal = []
	wall_normal += [ab[1] * ac[2] - ab[2] * ac[1]]
	wall_normal += [ab[2] * ac[0] - ab[0] * ac[2]]
	wall_normal += [ab[0] * ac[1] - ab[1] * ac[0]]
	
	wn = wall_normal
	l = (wn[0]**2 + wn[1]**2 + wn[2]**2 )**0.5
	
	wall_normals += [[wall_normal[0]/l, wall_normal[1]/l, wall_normal[2]/l]] 


plik.write('float Normals[] = {\n')

for vertex_index in range(len(vertices)):
	v=[0, 0, 0]
	counter = 0
	to_check = []
	#3 down triangles
	start = (vertex_index - 1) * 2 - 2 * vertex_index/(X_RANGE - 1) 
	stop = start + 3
	to_check += range(start,stop)
	#3 up triangles
	start = (vertex_index - 1 - X_RANGE) * 2
	stop = start + 3
	to_check += range(start, stop)
	#to_check.remove!!! remove all x < 0
	
	for triangle_index in range(len(indices)):
		if vertex_index in indices[triangle_index]:
			v[0] += wall_normals[triangle_index][0]
			v[1] += wall_normals[triangle_index][1]
			v[2] += wall_normals[triangle_index][2]
			counter += 1
			if counter == 6: #max 8 triangles
				break;
		
	#vertices_normals += [v]
	plik.write('\t' + str(v[0]) + ',\t' + str(v[1]) + ',\t' + str(v[2]) + ',\n')		
		
plik.write('};\n')

plik.close()
print("End with success!")
