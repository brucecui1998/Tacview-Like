#include "HudRenderer.h"
#include <GL/gl.h>

void HudRenderer::drawCompass(float rotY, int w, int h) {
    if (w <= 0 || h <= 0) return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);  // ✅ 保存所有状态
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glTranslatef(w - 60, h - 60, 0);
    glRotatef(-rotY, 0, 0, 1);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 20);
    glVertex2f(-10, -10);
    glVertex2f(10, -10);
    glEnd();

    glPopAttrib();  // ✅ 恢复状态（包含颜色、深度、纹理）
    glPopMatrix();  // ✅ 恢复 MODELVIEW

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();  // ✅ 恢复 PROJECTION
    glMatrixMode(GL_MODELVIEW);
}