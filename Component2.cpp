#include "Component.h"


Component::Component(int id, wxPoint pos, vector<int> borders, vector<int> connectpoints)
{
    jsonId = id;
    centrePos = pos;
    isShown = true;
    for (int i = 0; i < 4; i++) {
        border[i] = borders[i];
    }

    for (int i = 0; i < 6; i++) {
        connectpoint[i] = connectpoints[i];
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

    Json::Value points = target["connectpoints"];
    for (int i = 0; i < 6; i++) {
        connectpoint[i] = points[i].asInt();
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

wxPoint Component::FindPoint(const wxPoint& mousePosition)
{
    int i = 0;
    int circleRadius = 4;
    wxPoint connectPoint;
    for (int i = 0; i <6; i=i+2) {
        connectPoint.x = connectpoint[i] + centrePos.x;
        connectPoint.y = connectpoint[i + 1] + centrePos.y;
        if ((mousePosition.x - connectPoint.x) * (mousePosition.x - connectPoint.x) +
            (mousePosition.y - connectPoint.y) * (mousePosition.y - connectPoint.y) <= circleRadius * circleRadius) {
            return connectPoint;
        }
    }
    connectPoint.x = -1;
    connectPoint.y = -1;
    return connectPoint;
}

vector<int> Component::GetPoint()
{
    int connectPoints[6];
    connectPoints[0] = connectpoint[0] + centrePos.x;
    connectPoints[1] = connectpoint[1] + centrePos.y;
    connectPoints[2] = connectpoint[2] + centrePos.x;
    connectPoints[3] = connectpoint[3] + centrePos.y;
    connectPoints[4] = connectpoint[4] + centrePos.x;
    connectPoints[5] = connectpoint[5] + centrePos.y;
    return vector<int>(connectPoints,connectPoints+6);
}