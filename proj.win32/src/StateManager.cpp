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
cc::DrawNode* d_node;
std::vector<cc::Vec2> points;
std::vector<cc::Vec2> uvs;
std::vector<cc::Color4F> colours;

cc::CustomCommand* custom_cmd;
cc::PrimitiveCommand p_cmd;

cc::V3F_C4B_T2F* verts;
cc::TrianglesCommand tri_cmd;
cc::TrianglesCommand::Triangles tri;
//unsigned short* indices;

cc::VertexBuffer* vertexBuffer;
cc::VertexData* vertexData;
cc::IndexBuffer* indexBuffer;
short* indices;
cc::Primitive* primitive;

cc::VertexStreamAttribute* pos_vs;
cc::VertexStreamAttribute* col_vs;
cc::VertexStreamAttribute* tex_vs;

cc::QuadCommand* qcmd;
cc::V3F_C4B_T2F_Quad pq;

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

void state::on_draw() {
    //cc::GLProgram* s = cc::GLProgram::createWithFilenames("basic.vert", "basic.frag");
    //scene->setGLProgram(cc::ShaderCache::getInstance()->getGLProgram(cc::GLProgram::SHADER_NAME_POSITION_COLOR));

    float gradientAlpha = 0.7;
    cc::Vec2 vertices[4];
    cc::Color4F colors[4];
    int nVertices = 0;

    vertices[nVertices] = cc::Vec2(0, 0);
    colors[nVertices++] = { 0, 1, 1, 0 };
    vertices[nVertices] = cc::Vec2(0, 0);
    colors[nVertices++] = { 1, 1, 0, 0 };
    vertices[nVertices] = cc::Vec2(0, 0);
    colors[nVertices++] = { 0, 1, 0, gradientAlpha };
    vertices[nVertices] = cc::Vec2(0, 0);
    colors[nVertices++] = { 0, 1, 0, gradientAlpha };

    //cc::ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //cc::ccGLBindTexture2D(player->getTexture()->getName());
    //cc::ccGLEnableVertexAttribs(cc::GL::VERTEX_ATTRIB_FLAG_POSITION | cc::GL::VERTEX_ATTRIB_FLAG_COLOR);

    glClearColor((cos(scene->time_since_startup * 4.0f) / 2.0f) + .5f, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor4f(1, 1, 1, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(40, 0, 0);
    glVertex3f(0, 40, 0);
    glEnd();
}

void state::init(SceneManager* scene_ref) {
    scene = scene_ref;
    scene->setGLProgram(cc::ShaderCache::getInstance()->getGLProgram(cc::GLProgram::SHADER_NAME_POSITION_COLOR));

    sock::init();
    input::init();

    create_state(s);

    custom_cmd = new cc::CustomCommand();

    FILE* f = fopen("terrain.t2d", "r");

    if (f != NULL) {
        const int max_buf = 1024;
        char buf[max_buf];
        fgets(buf, max_buf, f);

        float v_x = 0;
        float v_y = 0;
        std::string temp = "";
        bool s = false;
        for (int n = 0; n < max_buf; ++n) {
            char c = buf[n];
            if (c == '\0') break;
            if (c == ',') {
                if (s) {
                    v_y = std::stof(temp);
                    cc::Vec2 v;
                    v.x = v_x * 40.0f;
                    v.y = v_y * 40.0f;
                    v.x += 400;
                    v.y += 100;
                    points.push_back(v);

                    cc::Vec2 uv;
                    uv.x = 0;
                    uv.y = 1;
                    uvs.push_back(uv);

                    cc::Color4F c;
                    c.r = 1.0f;
                    c.g = 0.0f;
                    c.b = 1.0f;
                    c.a = 1.0f;
                    colours.push_back(c);

                    //if (points.size() >= 6) break;
                }else v_x = std::stof(temp);
                temp = "";
                s = !s;
            }else {
                temp += c;
            }
        }
    }

    fclose(f);

    player = cc::Sprite::create("HelloWorld.png");
    player->setPosition(player->getContentSize().width, player->getContentSize().height);
    player->setScale(.25f);
    player->retain();
    //scene->addChild(player, 1);

    auto physics_body = cc::PhysicsBody::createBox(cc::Size(player->getContentSize().width * player->getScaleX(), 
                                                            player->getContentSize().height * player->getScaleY()));
    player->setPhysicsBody(physics_body);

    scene->p_world->setDebugDrawMask(cc::PhysicsWorld::DEBUGDRAW_ALL);
    auto dphysics_body = cc::PhysicsBody::create();
    //dphysics_body->addShape(cc::PhysicsShapePolygon::create(&points[0], points.size()));

    d_node = cc::DrawNode::create();

    d_node->setPhysicsBody(dphysics_body);
    dphysics_body->setGravityEnable(false);
    dphysics_body->setDynamic(false);
    //scene->addChild(d_node, 1);
    
    verts = new cc::V3F_C4B_T2F[4];
    verts[0].vertices = cc::Vertex3F(0, 0, 0);
    verts[0].colors = cc::Color4B(255, 255, 0, 255);
    verts[0].texCoords = cc::Tex2F(0, 0);

    verts[1].vertices = cc::Vertex3F(400, 0, 0);
    verts[1].colors = cc::Color4B(255, 255, 0, 255);
    verts[1].texCoords = cc::Tex2F(0, 0);

    verts[2].vertices = cc::Vertex3F(0, 400, 0);
    verts[2].colors = cc::Color4B(255, 255, 0, 255);
    verts[2].texCoords = cc::Tex2F(0, 0);

    verts[0] = player->getQuad().tl;
    verts[1] = player->getQuad().tr;
    verts[2] = player->getQuad().br;
    verts[3] = player->getQuad().bl;

    indices = new short[] {0, 1, 2};
    tri.indices = (unsigned short*)indices;
    tri.indexCount = 3;
    tri.verts = verts;
    tri.vertCount = 3;

    vertexBuffer = cc::VertexBuffer::create(sizeof(cc::V3F_C4B_T2F), 3);
    vertexBuffer->retain();
    vertexBuffer->updateVertices(&verts, 3, 0);

    vertexData = cc::VertexData::create();
    vertexData->retain();
    vertexData->setStream(vertexBuffer, cc::VertexStreamAttribute(0, cc::GLProgram::VERTEX_ATTRIB_POSITION, GL_FLOAT, 3, false));
    vertexData->setStream(vertexBuffer, cc::VertexStreamAttribute(12, cc::GLProgram::VERTEX_ATTRIB_COLOR, GL_UNSIGNED_BYTE, 4, true));
    vertexData->setStream(vertexBuffer, cc::VertexStreamAttribute(16, cc::GLProgram::VERTEX_ATTRIB_TEX_COORD, GL_FLOAT, 2, false));

    indexBuffer = cc::IndexBuffer::create(cc::IndexBuffer::IndexType::INDEX_TYPE_SHORT_16, 3);
    indexBuffer->retain();
    indexBuffer->updateIndices((unsigned short*)indices, 3, 0);

    primitive = cc::Primitive::create(vertexData, NULL, GL_TRIANGLES);
    primitive->retain();
    primitive->setStart(0);
    primitive->setCount(3);

    p_cmd.init(0.0f, player->getTexture()->getName(), player->getGLProgramState(),
        player->getBlendFunc(), primitive, player->getNodeToWorldTransform(), 1);
    cc::Director::getInstance()->getRenderer()->addCommand(&p_cmd);

    tri_cmd.init(0.0f, player->getTexture()->getName(), player->getGLProgramState(), 
        player->getBlendFunc(), tri, player->getNodeToWorldTransform(), 1);

    //player->draw(cc::Director::getInstance()->getRenderer(), player->getNodeToWorldTransform(), 1);

    //cc::Director::getInstance()->getRenderer()->addCommand(&tri_cmd);
}

void state::update(float dt) {
    p_cmd.init(0.0f, player->getTexture()->getName(), player->getGLProgramState(),
               player->getBlendFunc(), primitive, player->getNodeToWorldTransform(), 1);
    cc::Director::getInstance()->getRenderer()->addCommand(&p_cmd);
    int dv = cc::Director::getInstance()->getRenderer()->getDrawnVertices();

    player->getPhysicsBody()->applyForce(cc::Vec2(0, -2000.0f));
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(10000.0f, 0));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(-10000.0f, 0));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_UP_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(0, 40000.0f));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_DOWN_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(0, -40000.0f));
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