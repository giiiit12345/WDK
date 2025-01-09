#pragma once
#include <wx/wxprec.h>
#include "json.h"
#include <vector>
#include "json_ope.h"
#include <string>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class Component : public wxObject
{
public:
	Component() {};

	Component(int id, wxPoint pt, vector<int> border, const std::string& componentType);
	Component(Json::Value target);
	~Component() {}

	bool isSelected(const wxPoint& point);
	void Draw(wxMemoryDC& memDC, Json::Value root);

	wxPoint GetPos() const { return centrePos; }
	void Move(const wxPoint& pos) { centrePos = pos; }

	int GetId() const { return jsonId; }
	void SetId(int id) { jsonId = id; }

	vector<int> GetBorder() const { return vector<int>(border, border + 4); }
	wxRect GetRect() { return wxRect(centrePos.x + border[0], centrePos.y + border[1], border[2] - border[0], border[3] - border[1]); }

	bool IsShown() const { return isShown; }
	void SetShown(bool show) { isShown = show; }

	// 获取元器件类型
	std::string GetType() const; 

	// 设置元器件类型
	void SetType(const std::string& newType); 


	wxPoint centrePos; //元件中心点的位置
	int jsonId; //对应元件在json文件中的id
	int border[4]; //保存元件四周边框与中心点的相对位置
	bool isShown;
	std::string type;


};


