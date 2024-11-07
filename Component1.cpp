#include "Component.h"


Component::Component(int id, wxPoint pos, vector<int> borders)
{
    jsonId = id;
    centrePos = pos;
    isShown = true;
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