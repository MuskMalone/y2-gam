using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Image
{
    public static class GraphicsWrapper
    {
        public static void EmitterIncrementPosition(uint entity, Vector2 inc, int idx)
        {
            uint e = entity;
            Vector2 i = new Vector2(inc.X, inc.Y);
            int index = idx;
            InternalCalls.EmitterComponent_IncrementPosition(ref e, ref i, ref index);
        }
        public static void EmitterSetAlpha(uint entity, float alpha, int idx)
        {
            uint e = entity;
            float a = alpha;
            int index = idx;
            InternalCalls.EmitterComponent_SetAlpha(ref e, ref a, ref index);
        }
        public static void EmitterSetPosition(uint entity, Vector2 pos, int idx)
        {
            uint e = entity;
            Vector2 i = new Vector2(pos.X, pos.Y);
            int index = idx;
            InternalCalls.EmitterComponent_SetPosition(ref e, ref i, ref index);
        }
        public static void EmitterSetScale(uint entity, Vector2 scale, int idx)
        {
            uint e = entity;
            Vector2 i = new Vector2(scale.X, scale.Y);
            int index = idx;
            InternalCalls.EmitterComponent_SetScale(ref e, ref i, ref index);
        }
    }
}
