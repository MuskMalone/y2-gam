


#ifndef RECT_H
#define RECT_H

#include <glad/glad.h>

//#include "Circle.h"   // Circle class
//#include "Color.h"    // Color class
//#include "Config.h"   // Global vars, screen size
//#include "Utility.h"  // assignColor()
#include "Math/Vec2.hpp" 
#include "Math/Vec4.hpp"
#include "Math/Mat44.hpp"// Vec2 class
#include <Components/Camera.hpp>
#include <Systems/RenderSystem.hpp>
#include <Components/Transform.hpp>
#include <Components/Renderable.hpp>
#include "Core/Types.hpp"
#include <Core/Coordinator.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace DataMgmt {

	class Rect {
	private:
		//stores min and max of screen
		Vec2 min, max;
		//Color m_color;

	public:
		Rect() : min(Vec2(0, 0)), max(Vec2(0, 0)) {}
		Rect(const Vec2& a, const Vec2& b) : min{ a }, max{ b } {
		}
		void Draw() const { // for debugging remove later
			auto& camera = Coordinator::GetCoordinator()->GetComponent<tCamera>(Coordinator::GetCoordinator()->GetSystem<RenderSystem>()->GetCamera());

			Vec4 v4Min{ min, 0, 1 }, v4Max{ max, 0, 1 };
			Mat44 xform{ Coordinator::GetCoordinator()->GetSystem<RenderSystem>()->mLastModelXform };
			//v4Min = glm::inverse(xform) * v4Min;
			//v4Max = glm::inverse(xform) * v4Max;
			//glColor3ub(0xff, 0xff, 0xff);
			//glLineWidth(1.0);
			//glBegin(GL_LINE_LOOP);
			//glVertex2f(v4Min.x, v4Min.y);
			//glVertex2f(v4Min.x, v4Max.y);
			//glVertex2f(v4Max.x, v4Max.y);
			//glVertex2f(v4Max.x, v4Min.y);
			//glEnd();
		}

		template <typename _pred>
		bool contain(Entity const& id, _pred predicate) const {
			//Vec2 o = object_vec[id]->get_pos();
			//float r = object_vec[id]->get_radi();

			////  basic square collision check
			//if (o.x - r < max.x && o.x + r > min.x && o.y - r < max.y &&
			//    o.y + r > min.y) {
			//  return true;
			//}

			return predicate(id, * this);
		}
		bool contain_rect(const Rect& r) const {
			Vec2 rmin = r.get_min();
			Vec2 rmax = r.get_max();

			//  basic square collision check
			if (rmax.x < max.x && rmin.x > min.x && rmax.y < max.y && rmin.y > min.y) {
				return true;
			}

			return false;
		}
		bool contain_pos(const Vec2& v) const {
			if (v.x < min.x || v.x > max.x) return false;
			if (v.y < min.y || v.y > max.y) return false;

			return true;
		}

		Vec2 get_min() const { return min; }
		Vec2 get_max() const { return max; }
	};

}

#endif
