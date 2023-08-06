#pragma once
#include "Vector3.h"
#include <iostream>
#include <fstream>
#include <variant>
#include <string>
#include <map>
#include <json.hpp>

class GlobalVariables {
public:
	using json = nlohmann::json;

	//項目
	struct Item {
		//項目の値
		std::variant<int32_t, float, Vector3> value;
	};

	//グループ
	struct Group {
		std::map<std::string, Item> items;
	};

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static GlobalVariables* GetInstance();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	//値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	//値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	//値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

public:
	const std::string kDirectoryPath = "Resources/GlobalVariables/";

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

private:
	// 全データ
	std::map<std::string, Group> datas_;
};