#include "map/Terrain.h"

#include <2d/CCSprite.h>
#include <base/CCDirector.h>
#include <physics/CCPhysicsBody.h>
#include <renderer/CCTextureCache.h>
#include <renderer/CCTrianglesCommand.h>
#include <renderer/CCRenderer.h>
#include <2d/CCCamera.h>
#include <renderer/CCGLProgram.h>
#include <renderer/CCGLProgramCache.h>

#include "assets/Textures.h"
#include "debug/Logger.h"
#include "entities/units/Unit.h"
#include "map/Cam.h"
#include "StateManager.h"

BEGIN_MAP_NS
BEGIN_TERRAIN_NS

using namespace cocos2d;

//private

//public
//-- begin terrain class --

Terrain::Terrain(TerrainData& t_data, TerrainGroup& t_group) {
    terrain_data = &t_data;

    base = Node::create();

    if (t_data.collider_points.size() >= 1) {
        pbody = PhysicsBody::createEdgePolygon(&t_data.collider_points[0], t_data.collider_points.size(), PHYSICSBODY_MATERIAL_DEFAULT, 2.0f);
        pbody->setDynamic(false);
        pbody->setCollisionBitmask(1);
        pbody->setContactTestBitmask(true);
        pbody->setPositionOffset(Vec2(0, -400.0f));

        base->setPhysicsBody(pbody);
        //base->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        //base->setScale(1.1f, 1.1f);
        has_collider = true;
    }
    base->setPosition(t_data.pos * 20.0f);
    base->retain();
    t_group.base->addChild(base, 1);

    //init edge_tris, fill_tris and commands for both of them
    edge_tris.indices = &t_data.indices[t_data.edge_indices_start];
    edge_tris.indexCount = t_data.edge_indices_end;
    edge_tris.verts = &t_data.points[0];
    edge_tris.vertCount = t_data.points.size();

    Texture2D* edge_t = Director::getInstance()->getTextureCache()->addImage("ForestEdges.png");
    edge_t->retain();
    edge_t->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

    BlendFunc blend_func;
    blend_func.src = GL_SRC_ALPHA;
    blend_func.dst = GL_ONE_MINUS_SRC_ALPHA;

    edge_tris_cmd.init(0.0f, edge_t->getName(), root::scene->getGLProgramState(), blend_func, edge_tris, base->getNodeToWorldTransform());

    fill_tris.indices = &t_data.indices[t_data.fill_indices_start];
    fill_tris.indexCount = t_data.fill_indices_end;
    fill_tris.verts = &t_data.points[0];
    fill_tris.vertCount = t_data.points.size();

    Texture2D* fill_t = Director::getInstance()->getTextureCache()->addImage("ForestFill.png");
    fill_t->retain();
    fill_t->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

    fill_tris_cmd.init(0.0f, fill_t->getName(), root::scene->getGLProgramState(), blend_func, fill_tris, base->getNodeToWorldTransform());

    //turn on/off debug geometry depending on default value
    debug_draw_node = DrawNode::create();
    debug_draw_node->retain();

    debug_draw_on = !debug_draw_on;
    toggle_debug_geometry();
}

void Terrain::test_explosion_at(Vec2 vec) {
    auto t = base->nodeToWorldTransform();
    auto p = base->getPosition();

    fill_tris.verts[0].vertices.x = vec.x;
    fill_tris.verts[0].vertices.y = vec.y;
}

void Terrain::create_debug_geometry(bool show_triangles, bool show_collider_points) {
    base->addChild(debug_draw_node);

    if (show_triangles) {
        for (int n = 1; n < terrain_data->indices.size(); ++n) {
            if (n % 3 == 0) {
                debug_draw_node->drawLine(terrain_data->debug_points[terrain_data->indices[n - 1]],
                                          terrain_data->debug_points[terrain_data->indices[n - 3]],
                                          Color4F(1.0f, 1.0f, 1.0f, .4f));
            }else {
                debug_draw_node->drawLine(terrain_data->debug_points[terrain_data->indices[n - 1]],
                                          terrain_data->debug_points[terrain_data->indices[n]],
                                          Color4F(1.0f, 1.0f, 1.0f, .4f));
            }
        }
    }

    if (show_collider_points) {
        for (int n = 1; n < terrain_data->collider_points.size(); ++n) {
            debug_draw_node->drawLine(terrain_data->collider_points[n - 1],
                                      terrain_data->collider_points[n],
                                      Color4F(0.0f, 0.0f, 1.0f, .8f));
        }
    }
}

void Terrain::remove_debug_geometry() {
    base->removeChild(debug_draw_node);
    debug_draw_node->clear();
}

void Terrain::show_debug_geometry(bool show_triangles, bool show_collider_points) {
    create_debug_geometry(show_triangles, show_collider_points);
}

void Terrain::hide_debug_geometry() {
    remove_debug_geometry();
}

void Terrain::toggle_debug_geometry() {
    if (debug_draw_on = !debug_draw_on) show_debug_geometry();
    else hide_debug_geometry();
}

void Terrain::draw() {
    fill_tris_cmd._mv = base->getNodeToWorldTransform();
    edge_tris_cmd._mv = base->getNodeToWorldTransform();
    if (has_collider) pbody->setPositionOffset(base->getPosition());
    debug_draw_node->setPosition(base->getPosition());
    Director::getInstance()->getRenderer()->addCommand(&fill_tris_cmd);
    Director::getInstance()->getRenderer()->addCommand(&edge_tris_cmd);
}

//-- end terrain class --

//-- begin TerrainGroup class --

TerrainGroup::TerrainGroup(TerrainDataGroup* data_group) {
    base = Node::create();
    root::map_layer->addChild(base);
    for (int n = 0; n < data_group->data_vec.size(); ++n) {
        auto t = new Terrain(*data_group->data_vec[n].get(), *this);
        terrain_list.push_back(t);
    }
}

void TerrainGroup::show_debug_geometry(bool show_triangles, bool show_collider_points) {
    debug_draw_on = true;
    for (int n = 0; n < terrain_list.size(); ++n) {
        terrain_list[n]->show_debug_geometry(show_triangles, show_collider_points);
    }
}

void TerrainGroup::hide_debug_geometry() {
    debug_draw_on = false;
    for (int n = 0; n < terrain_list.size(); ++n) {
        terrain_list[n]->hide_debug_geometry();
    }
}

void TerrainGroup::toggle_debug_geometry() {
    debug_draw_on = !debug_draw_on;
    for (int n = 0; n < terrain_list.size(); ++n) {
        if (debug_draw_on)  terrain_list[n]->show_debug_geometry();
        else                terrain_list[n]->hide_debug_geometry();
    }
}

void TerrainGroup::draw() {
    for (int n = 0; n < terrain_list.size(); ++n) {
        terrain_list[n]->draw();
    }
}

Terrain* TerrainGroup::is_terrain(Node* a, Node* b) {
    for (int n = 0; n < terrain_list.size(); ++n) {
        if (terrain_list[n]->base == a || terrain_list[n]->base == b) return terrain_list[n];
    }
    return NULL;
}

//-- end TerrainGroup class --

END_TERRAIN_NS
END_MAP_NS