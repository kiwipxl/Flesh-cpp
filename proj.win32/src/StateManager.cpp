#include "StateManager.h"
#include "debug/Errors.h"
#include "network/sockets/SocketManager.h"
#include "input/SimpleInput.h"
#include "network/Peers.h"
#include <stdio.h>

using state::State;

SceneManager* state::scene;

State state::s = state::SERVER_CONNECT_SCREEN;
cc::LabelBMFont* state::info_label;
float state::time_since_startup = 0;
cc::TextFieldDelegate username_input;

cc::Sprite* state::player;
bool can_jump = false;

cc::Camera* camera;

void create_state(State c_state) {
    using namespace state;

    s = c_state;
    switch (s) {
        case SERVER_CONNECT_SCREEN:
            info_label = cc::LabelBMFont::create("", "fonts/lucida.fnt");
            info_label->setString("connecting...");
            //scene->addChild(info_label, 1);

            scene->scheduleUpdate();

            sock::setup_tcp_sock();
            break;
        case LOGIN_REGISTER_SCREEN:
            break;
    }
}

void remove_state(State r_state) {
    using namespace state;

    s = r_state;
    switch (s) {
        case SERVER_CONNECT_SCREEN:
            scene->removeChild(info_label);
            break;
    }
}

void state::switch_state(State new_state) {
    remove_state(s);
    create_state(new_state);
}

void state::init(SceneManager* scene_ref) {
    scene = scene_ref;
    scene->setGLProgram(cc::ShaderCache::getInstance()->getGLProgram(cc::GLProgram::SHADER_NAME_POSITION_COLOR));

    sock::init();
    input::init();

    create_state(s);

    player = cc::Sprite::create("HelloWorld.png");
    player->setPosition(player->getContentSize().width, player->getContentSize().height);
    player->setScale(.25f);
    player->retain();
    scene->addChild(player, 1);

    auto physics_body = cc::PhysicsBody::createBox(cc::Size(player->getContentSize().width * player->getScaleX(), 
                                                            player->getContentSize().height * player->getScaleY()));
    player->setPhysicsBody(physics_body);

    //scene->p_world->setDebugDrawMask(cc::PhysicsWorld::DEBUGDRAW_ALL);

    auto dphysics_body = cc::PhysicsBody::createEdgePolygon(&collider_points[0], collider_points.size());
    dphysics_body->setGravityEnable(false);
    dphysics_body->setDynamic(false);
    dphysics_body->setCategoryBitmask(0x02);
    dphysics_body->setCollisionBitmask(0x04);
    dphysics_body->setContactTestBitmask(0x04);
    physics_body->setCategoryBitmask(0x04);
    physics_body->setCollisionBitmask(0x02);
    physics_body->setContactTestBitmask(0x02);
    terrain_node->setPhysicsBody(dphysics_body);
    scene->addChild(terrain_node, 1);
    d_node->retain();

    edge_tris.indices = &indices[edge_indices_start];
    edge_tris.indexCount = edge_indices_end;
    edge_tris.verts = &points[0];
    edge_tris.vertCount = points.size();

    cc::Texture2D* edge_t = cc::Director::getInstance()->getTextureCache()->addImage("MossyEdges.png");
    edge_t->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

    edge_tris_cmd.init(0.0f, edge_t->getName(), player->getGLProgramState(),
                       player->getBlendFunc(), edge_tris, terrain_node->getNodeToWorldTransform(), 1);

    fill_tris.indices = &indices[fill_indices_start];
    fill_tris.indexCount = fill_indices_end;
    fill_tris.verts = &points[0];
    fill_tris.vertCount = points.size();

    cc::Texture2D* fill_t = cc::Director::getInstance()->getTextureCache()->addImage("MossyFill.png");
    fill_t->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

    fill_tris_cmd.init(0.0f, fill_t->getName(), player->getGLProgramState(),
                       player->getBlendFunc(), fill_tris, terrain_node->getNodeToWorldTransform(), 1);

    auto spritePos = cc::Vec3(scene->screen_size.width / 2 + 0,
        scene->screen_size.height / 2 + 0,
        0);

    // position the sprite on the center of the screen
    //this is the layer, when adding camera to it, all its children will be affect only when you set the second parameter to true
    scene->setCameraMask((unsigned short)cc::CameraFlag::USER1, true);
    // add the sprite as a child to this layer

    camera = cc::Camera::createPerspective(60, (float)scene->screen_size.width / scene->screen_size.height, 1.0, 1000);
    camera->setCameraFlag(cc::CameraFlag::USER1);
    //the calling order matters, we should first call setPosition3D, then call lookAt.
    camera->setPosition3D(spritePos + cc::Vec3(0, 0, 800));
    camera->lookAt(spritePos, cc::Vec3(0.0, 1.0, 0.0));
    scene->addChild(camera);
}

void state::update(float dt) {
    cc::Vec3 pos;
    pos.x = player->getPositionX();
    pos.y = player->getPositionY();
    camera->setPosition3D(pos + cc::Vec3(0, 0, 600));
    camera->lookAt(pos, cc::Vec3(0.0, 1.0, 0.0));

    cc::Director::getInstance()->getRenderer()->addCommand(&fill_tris_cmd);
    cc::Director::getInstance()->getRenderer()->addCommand(&edge_tris_cmd);
    d_node->draw(cc::Director::getInstance()->getRenderer(), terrain_node->getNodeToWorldTransform(), 0);

    player->getPhysicsBody()->applyImpulse(cc::Vec2(0, -10000.0f));
    player->getPhysicsBody()->setVelocityLimit(500.0f);
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(10000.0f, 0));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(-10000.0f, 0));
    }
    if (can_jump && input::key_down(cc::EventKeyboard::KeyCode::KEY_UP_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(0, 40000.0f));
        can_jump = false;
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_DOWN_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(0, -40000.0f));
    }
    int a = player->getPhysicsBody()->getContactTestBitmask();
    int b = 0;
    if (a != 0) {
        can_jump = true;
    }

    for (int n = 0; n < peers::peer_list.size(); ++n) {
        msg::send(*peers::peer_list[n]->udp_sock, msg::ByteStream() << _MID->PO_PLAYER_MOVEMENT << 
            (int)player->getPositionX() << (int)player->getPositionY());
    }

    return;

    switch (s) {
        case SERVER_CONNECT_SCREEN:
            sock::update();

            info_label->setPosition(cc::Vec2((cos(time_since_startup) * 40.0f) + 400, 200));

            if (sock::connection_finished) {
                if (sock::connection_error == NO_ERROR) {
                    switch_state(LOGIN_REGISTER_SCREEN);
                }else {
                    info_label->setString("an error occurred while trying to connect: " + SSTR(sock::connection_error));
                }
            }else {
                info_label->setString("connecting to server...");
            }
            break;
    }
}