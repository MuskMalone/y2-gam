


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
#include <Components/Sprite.hpp>
#include "Core/Types.hpp"
#include <Core/Coordinator.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <Graphics/Renderer.hpp>
#include <Components/OrthoCamera.hpp>

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


			Vec4 v4Min{ min, 0, 1 }, v4Max{ max, 0, 1 };
			Vec4 v4Scale{ v4Max - v4Min };

			Renderer::DrawLineRect((v4Min + v4Scale / 2.f), v4Scale, { 1.f, 0.5f, 0.2f ,1.f });


		}

		template <typename _pred>
		bool Contain(Entity const& id, _pred predicate) const {
			//Vec2 o = object_vec[id]->get_pos();
			//float r = object_vec[id]->get_radi();

			////  basic square collision check
			//if (o.x - r < max.x && o.x + r > min.x && o.y - r < max.y &&
			//    o.y + r > min.y) {
			//  return true;
			//}

			return predicate(id, * this);
		}
		bool ContainRect(const Rect& r) const {
			Vec2 rmin = r.GetMin();
			Vec2 rmax = r.GetMax();

			//  basic square collision check
			if (rmax.x < max.x && rmin.x > min.x && rmax.y < max.y && rmin.y > min.y) {
				return true;
			}

			return false;
		}
		bool ContainPos(const Vec2& v) const {
			if (v.x < min.x || v.x > max.x) return false;
			if (v.y < min.y || v.y > max.y) return false;

			return true;
		}

		Vec2 GetMin() const { return min; }
		Vec2 GetMax() const { return max; }
	};

}

#endif
