// PointsCheck.cpp: определяет точку входа для приложения.
//

#include "PointsCheck.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
using namespace std;

vector<string> ReadFileError(string path);
vector<string> ReadFile(string path);
void Standardization(vector<vector<float>> coordinates);
void Calculate(vector<vector<float>> points, vector<vector<float>> plains);
string CoordinateToString(vector<float> coordinate);
vector<vector<float>> ConvertString(vector<string> str_coordinates);
vector<float> CreateVectorFromToPoints(vector<float> point1,
	vector<float> point2);
bool IsGoodPoint(
	vector<float> plane1_point, vector<float> plane1_v1, vector<float> plane1_v2,
	bool relate_zero1,
	vector<float> plane2_point, vector<float> plane2_v1, vector<float> plane2_v2,
	bool relate_zero2,
	vector<float> point_forCheck);
float RelatePointWithPlane(vector<float> point_on_plane,
	vector<float> v1, vector<float> v2,
	vector<float> point_for_check);
string CreatePathName(string old_path, string old_ending, string new_ending);
void WriteFile(string path, vector<string> content);
vector<string> ConvertFloatsToStrings(vector<vector<float>> points);

vector<vector<float>> GoodPoints;
vector<vector<float>> WrongPoints;

//int main(string path_points, string path_plains)
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "invalid parameters number" << endl;
	}
	string path_points = argv[1];
	string path_plains = argv[2];
	//setlocale(LC_ALL, "");
	// --TEST-START--
	//string path_points = "C:\\Sol\\Solution\\points.txt";
	//string path_plains = "C:\\Sol\\Solution\\planes.txt";
	// --TEST-END--

	vector<string> str_points = ReadFileError(path_points);
	vector<string> str_plains = ReadFileError(path_plains);

	if (str_points.size() <= 0
		|| str_plains.size() <= 0)
	{
		return 1;
	}

	vector<vector<float>> points = ConvertString(str_points);
	vector<vector<float>> plains = ConvertString(str_plains);

	Calculate(points, plains);

	string path_good = CreatePathName(path_points, ".txt", "_good.txt");
	string path_wrong = CreatePathName(path_points, ".txt", "_wrong.txt");;

	WriteFile(path_good, ConvertFloatsToStrings(GoodPoints));
	WriteFile(path_wrong, ConvertFloatsToStrings(WrongPoints));

	return 0;
}

/// <summary>
/// Определить корректные и ложные точки.
/// Записать значения в GoodPoints и WrongPoints.
/// </summary>
/// <param name="points"></param>
/// <param name="plains"></param>
void Calculate(vector<vector<float>> points, vector<vector<float>> plains)
{
	Standardization(points);
	Standardization(plains);

	if (plains.size() != 6)
	{
		cout << "Wrong points number for plains definition." << endl;
		return;
	}

	// Построить 2 вектора для первой плоскости.
	vector<float> v1 = CreateVectorFromToPoints(plains[0], plains[1]);
	vector<float> v2 = CreateVectorFromToPoints(plains[0], plains[2]);
	// Построить 2 вектора для второй плоскости.
	vector<float> v3 = CreateVectorFromToPoints(plains[3], plains[4]);
	vector<float> v4 = CreateVectorFromToPoints(plains[3], plains[5]);

	vector<float> zero = { 0, 0, 0 };
	bool relate_zero1 = RelatePointWithPlane(plains[0], v1, v2, zero) <= 0;
	bool relate_zero2 = RelatePointWithPlane(plains[3], v3, v4, zero) <= 0;

	for (vector<float>& point : points)
	{
		if (IsGoodPoint(plains[0], v1, v2, relate_zero1,
			plains[3], v3, v4, relate_zero2,
			point))
		{
			GoodPoints.push_back(point);
			continue;
		}
		WrongPoints.push_back(point);
	}
}

vector<string> ConvertFloatsToStrings(vector<vector<float>> points)
{
	vector<string> str_points;
	for (vector<float> point : points)
	{
		// Без избыточных проверок на размер списка,
		// т.к. на данном этапе сюда не должны попасть списки неправильного размера.
		str_points.push_back(
			to_string(point[0]) + " "
			+ to_string(point[1]) + " "
			+ to_string(point[2]));
	}

	return str_points;
}

bool IsGoodPoint(
	vector<float> plane1_point, vector<float> plane1_v1, vector<float> plane1_v2,
	bool relate_zero1,
	vector<float> plane2_point, vector<float> plane2_v1, vector<float> plane2_v2,
	bool relate_zero2,
	vector<float> point_forCheck
)
{
	// Находится с той же стороны от плоскостей, что и начало координат.
	return
		(RelatePointWithPlane(plane1_point, plane1_v1, plane1_v2, point_forCheck)
			<= 0 == relate_zero1)
		&& (RelatePointWithPlane(plane2_point, plane2_v1, plane2_v2, point_forCheck)
			<= 0 == relate_zero2);
}

/// <summary>
/// Построить матрицу и подсчитать её определитель.
/// </summary>
/// <param name="point_on_plane"></param>
/// <param name="vector1"></param>
/// <param name="vector2"></param>
/// <param name="points_for_check"></param>
/// <returns></returns>
float RelatePointWithPlane(vector<float> point_on_plane,
	vector<float> v1, vector<float> v2,
	vector<float> point_for_check)
{
	float x = point_for_check[0] - point_on_plane[0];
	float y = point_for_check[1] - point_on_plane[1];
	float z = point_for_check[2] - point_on_plane[2];

	return
		x * (v1[1] * v2[2] - v1[2] * v2[1])
		- y * (v1[0] * v2[2] - v1[2] * v2[0])
		+ z * (v1[0] * v2[1] - v1[1] * v2[0]);
}

void DefineEuclideanPlane(vector<float> point1,
	vector<float> point2, vector<float> point3)
{
	// Построить 2 вектора
	vector<float> v1 = CreateVectorFromToPoints(point1, point2);
	vector<float> v2 = CreateVectorFromToPoints(point1, point3);


}

vector<float> CreateVectorFromToPoints(vector<float> point1,
	vector<float> point2)
{
	vector<float> result;

	result.push_back(point1[0] - point2[0]);
	result.push_back(point1[1] - point2[1]);
	result.push_back(point1[2] - point2[2]);

	return result;
}

vector<vector<float>> ConvertString(vector<string> str_coordinates)
{
	vector<vector<float>> result;
	// 
	for (string str_coordinate : str_coordinates)
	{
		vector<string> vector_coordinate;

		string tmp = "";
		for (int i = 0; i <= str_coordinate.length(); i++)
		{
			if (str_coordinate[i] == ' ') {
				vector_coordinate.push_back(tmp);
				tmp.clear();
			}
			/*if (str_coordinate[i] == '.') {
				tmp += ',';
				continue;
			}*/
			tmp += str_coordinate[i];
		}
		vector_coordinate.push_back(tmp);
		tmp.clear();

		vector<float> float_values;
		for (string str_value : vector_coordinate)
		{
			char* ending;
			float_values.push_back(strtof(str_value.c_str(), &ending));
			if (*ending != 0) {
				cout << "Can not convert " + str_value + " to float." << endl;
			}
		}
		result.push_back(float_values);
		float_values.clear();
	}
	return result;
}

/// <summary>
/// Привести список с координатами к стандартному виду:
/// в каждом vector<float> должно быть ровно 3 числа.
/// </summary>
/// <param name="coordinates"></param>
void Standardization(vector<vector<float>> coordinates)
{
	for (vector<float> coordinate : coordinates)
	{
		if (coordinate.size() == 3)
		{
			continue;
		}
		cout << "Wrong string: " + CoordinateToString(coordinate) << endl;

		while (coordinate.size() < 3)
		{
			coordinate.push_back(0);
		}

		while (coordinate.size() > 3)
		{
			coordinate.pop_back();
		}
	}
}

string CoordinateToString(vector<float> coordinate)
{
	string result = "";
	for (float value : coordinate)
	{
		result += " " + to_string(value);
	}
	return result;
}

string CreatePathName(string old_path, string old_ending, string new_ending)
{
	string result;
	// Получить окончание пути.
	string ending
		= old_path.substr(old_path.size() - old_ending.size(), old_ending.size());
	// Сравнить окончание с переданным.
	if (ending == old_ending)
	{
		result = old_path.substr(0, old_path.size() - old_ending.size())
			+ new_ending;
	}
	else
	{
		result = old_path + new_ending;
	}
	return result;
}

/// <summary>
/// Вызов функции чтения файла и проверка результата на ошибку.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
vector<string> ReadFileError(string path)
{
	vector<string> result = ReadFile(path);
	if (result.size() <= 0)
	{
		cout << "Can not read information from file " + path << endl;
	}
	return result;
}

void WriteFile(string path, vector<string> content)
{
	fstream out;
	out.open(path, fstream::out);
	if (out.is_open())
	{
		for (string str_point : content)
		{
			out << str_point << endl;
		}
	}

	out.close();
}

/// <summary>
/// 
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
vector<string> ReadFile(string path)
{
	vector<string> all_lines = vector<string>();
	if (!filesystem::exists(path))
	{
		return all_lines;
	}

	ifstream input(path);
	if (input.is_open())
	{
		string line = "";
		while (getline(input, line))
		{
			all_lines.push_back(line);
		}
	}
	input.close();
	return all_lines;
}
