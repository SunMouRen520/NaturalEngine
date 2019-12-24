#include "utils.h"
#include <random>

void initializePlaneVAO(const int res, const int width, GLuint * planeVAO, GLuint * planeVBO, GLuint * planeEBO)
{
	// const int res = 3
	const int nPoints = res * res;
	const int size = nPoints * 3 + nPoints * 3 + nPoints * 2;
	float * vertices = new float[size];
	for (int i = 0; i < res; i++)
	{
		for (int j = 0; j < res; j++)
		{
			// add position
			float x = j * (float)width / (res - 1) - width / 2.0;
			float y = 0.0;
			float z = -i * (float)width / (res - 1) - width / 2.0;

			vertices[(i + j * res) * 8] = x;	// 8 = 3 + 3 + 2, float per point 
			vertices[(i + j * res) * 8 + 1] = y;
			vertices[(i + j * res) * 8 + 2] = z;

			// add normal
			float x_n = 0.0;
			float y_n = 1.0;
			float z_n = 0.0;

			vertices[(i + j * res) * 8 + 3] = x_n;
			vertices[(i + j * res) * 8 + 4] = y_n;
			vertices[(i + j * res) * 8 + 5] = z_n;

			// add texcoords
			vertices[(i + j * res) * 8 + 6] = (float)j / (res - 1);
			vertices[(i + j * res) * 8 + 7] = (float)(res - i - 1) / (res - 1);

		}
	}

	const int nTris = (res - 1) * (res - 1) * 2;
	int * trisIndices = new int[nTris * 3];

	for (int i = 0; i < nTris; i++)
	{
		int trisPerRow = 2 * (res - 1);
		for (int j = 0; j < trisPerRow; j++)
		{
			if (!(i % 2))
			{
				// upper triangle
				int k = i * 3;
				int triIndex = i % trisPerRow;

				int row = i / trisPerRow;
				int col = triIndex / 2;
				trisIndices[k] = row * res + col;
				trisIndices[k + 1] = ++row * res + col;
				trisIndices[k + 2] = --row * res + ++col;
			}
			else
			{
				int k = i * 3;
				int triIndex = i % trisPerRow;

				int row = i / trisPerRow;
				int col = triIndex / 2;
				trisIndices[k] = row * res + ++col;
				trisIndices[k + 1] = ++row * res + --col;
				trisIndices[k + 2] = row * res + ++col;
			}
		}
	}

	glGenVertexArrays(1, planeVAO);
	glGenBuffers(1, planeVBO);
	glGenBuffers(1, planeEBO);

	glBindVertexArray(*planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, *planeVBO);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *planeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTris * 3 * sizeof(unsigned int), trisIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	delete[] vertices;

}

glm::vec3 genRandomVec3()
{
	// ���ڻ�ȡ���ӵ����������
	std::random_device rd;
	// ���rd()�ı�׼mersenne_twister_engine
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(.0, 100.);

	float x, y, z;
	x = dis(gen);
	y = dis(gen);
	z = dis(gen);

	return glm::vec3();
}
