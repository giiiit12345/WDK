#include "Component.h"


Component::Component(int id, wxPoint pos, vector<int> borders, const std::string& componentType)
{
    jsonId = id;
    centrePos = pos;
    isShown = true;
    type = componentType;  // 设置元器件类型
    for (int i = 0; i < 4; i++) {
        border[i] = borders[i];
    }
}

Component::Component(Json::Value target)
{
    wxString str = target["id"].asString();
    jsonId = atoi(str);
    centrePos = wxPoint(0, 0);
    isShown = true;
    type = target["name"].asString();
    Json::Value box = target["boundingBox"];
    for (int i = 0; i < 4; i++) {
        border[i] = box[i].asInt();
    }
}

bool Component::isSelected(const wxPoint& point)
{
    wxRect rect(GetRect());
    return rect.Contains(point.x, point.y);
}

void Component::Draw(wxMemoryDC& memDC, Json::Value root)
{
    DrawFromJson(root, memDC, centrePos.x, centrePos.y);
}

// 获取元器件类型
std::string Component::GetType() const
{
    return type;
}

// 设置元器件类型
void Component::SetType(const std::string& newType)
{
    type = newType;
}
