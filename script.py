def mathFunc(x, z):
	return (x * x + z * z)/20.0
	
def sumVec3(a, b):
	result = []
	for index in range(len(a)):
		result += [a[index] + b[index]]
	return result

vertices = []
indices = []
wall_normals = []
vertices_normals =[]

X_RANGE = 100
Y_RANGE = 10
SCALE = 100

plik = open('surface.h','w')

#Vertices

plik.write('#include <ode/ode.h>\n\nconst int VertexCount = '+str(X_RANGE*Y_RANGE)+';\nconst int IndexCount = '+str(2* (X_RANGE - 1)*(Y_RANGE - 1)) +';\n\n')

plik.write('float Vertices[] = {\n')

for x in range(X_RANGE):
	for z in range(Y_RANGE):
		y = mathFunc(x,z)
		vertices += [[SCALE *x, SCALE * y,SCALE * z]]
		plik.write('\t' + str(SCALE * x) + ',\t' + str(SCALE * y) + ',\t' + str(SCALE * z) + ',\n')

plik.write('};\n')#change to '/b/b/n}' but /b shuld be a backspace and /b doesn't work


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
		plik.write('\t{' + str(a_index) + ',\t' + str(b_index) + ',\t' + str(c_index) + '},\n')		
		indices +=[[a_index, b_index, c_index]]
		
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
	
	ab = [a[0] - b[0], a[1] - b[1], a[2] - b[2]]
	ac = [a[0] - c[0], a[1] - c[1], a[2] - c[2]]
	
	#wall_normals += 
	
	
plik.close()
print("End with success!")
