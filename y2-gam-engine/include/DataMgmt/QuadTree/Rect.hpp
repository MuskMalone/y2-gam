


#ifndef RECT_H
#define RECT_H

#include <glad/glad.h>

//#include "Circle.h"   // Circle class
//#include "Color.h"    // Color class
//#include "Config.h"   // Global vars, screen size
//#include "Utility.h"  // assignColor()
#include "Math/Vec2.hpp"     // Vec2 class
#include "Core/Types.hpp"
namespace DataMgmt {

	class Rect {
	private:
		//stores min and max of screen
		Vec2 min, max;
		//Color m_color;

	public:
		Rect() : min(Vec2(0, 0)), max(Vec2(0, 0)) {}
		Rect(const Vec2& a, const Vec2& b) {
			//assignColor(m_color);
			//if (use_fixedgrid) uniCol++;
		}

		void draw() const { // for debugging remove later
			glColor3ub(0xff, 0xff, 0xff);
			glLineWidth(1.0);
			glBegin(GL_LINE_LOOP);
			glVertex2f(min.x + 1, min.y + 1);
			glVertex2f(min.x + 1, max.y - 1);
			glVertex2f(max.x - 1, max.y - 1);
			glVertex2f(max.x - 1, min.y + 1);
			glEnd();
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
