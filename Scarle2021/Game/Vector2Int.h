#pragma once

/**
 * \brief Vector 2 of integers
 */
struct Vector2Int
{
	//Operator to allow comparison
	bool operator==(const Vector2Int& other) const
	{
		return x == other.x && y == other.y;
	}

	//Inverts comparison and links it to != operator 
	bool operator!=(const Vector2Int& other) const
	{
		return !(*this == other);
	}

	//Support for < operator
	bool operator<(const Vector2Int& other) const
	{
		const auto magnitude = GetMagnitude(other);
		return magnitude.first < magnitude.second;
	}

	//Support for < operator
	bool operator>(const Vector2Int& other) const
	{
		return !(*this < other);
	}

	//Support for <= operator
	bool operator<=(const Vector2Int& other) const
	{
		const auto magnitude = GetMagnitude(other);
		return magnitude.first <= magnitude.second;
	}

	//Support for >= operator
	bool operator>=(const Vector2Int& other) const
	{
		return !(*this <= other);
	}

	//Support for + operator 
	Vector2Int operator+(const Vector2Int& other) const
	{
		return Vector2Int{x + other.x, y + other.y};
	}

	//Support for - operator 
	Vector2Int operator-(const Vector2Int& other) const
	{
		return Vector2Int{x - other.x, y - other.y};
	}

	//Support for * operator 
	Vector2Int operator*(int multiplier) const
	{
		return Vector2Int{x * multiplier, y * multiplier};
	}

	//Support for / operator
	Vector2Int operator/(int divider) const
	{
		return Vector2Int{ x / divider, y / divider };
	}

	int x = 0;
	int y = 0;

private:

	//Returns the magnitude of 2 Vector2Int
	std::pair<int, int> GetMagnitude(const Vector2Int& other) const
	{
		const int mag_1 = sqrt(x * x + y * y);
		const int mag_2 = sqrt(other.x * other.x + other.y * other.y);

		return std::pair<int, int>(mag_1, mag_2);
	}
};