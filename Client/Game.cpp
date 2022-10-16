#include "pch.h"
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	//삼각형 만들기
	//vertex : 정점정보만 있음
	//index buffer: 삼각형 연결 정보

	//vertex만 사용
	/*vector<Vertex> vec(6);
	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);

	vec[3].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[4].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[4].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[5].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[5].color = Vec4(0.f, 0.f, 1.f, 1.f);*/


	vector<Vertex> vec(5);
	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(0.f, 1.f, 1.f, 1.f);
	vec[4].pos = Vec3(0.f, 1.f, 0.0f);
	vec[4].color = Vec4(1.f, 1.f, 0.f, 1.f);
	
	vector<uint32> indexBuffer;
	{
		indexBuffer.push_back(0);
		indexBuffer.push_back(1);
		indexBuffer.push_back(2);

	}
	{
		indexBuffer.push_back(0);
		indexBuffer.push_back(2);
		indexBuffer.push_back(3);

	}
	{
		indexBuffer.push_back(0);
		indexBuffer.push_back(4);
		indexBuffer.push_back(1);

	}


	mesh->Init(vec,indexBuffer);
	shader->Init(L"..\\Resources\\Shader\\default.hlsli");

	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	GEngine->RenderBegin();

	shader->Update();
	{
		Transform t;
		t.offset = Vec4(0.f, 0.f, 0.f, 0.f);
		mesh->SetTransform(t);
		mesh->Render();

	}
	//{
	//	Transform t;
	//	t.offset = Vec4(0.5f, 0.f, 0.f, 0.f);
	//	mesh->SetTransform(t);
	//	mesh->Render();

	//}

	//{
	//	Transform t;
	//	t.offset = Vec4(0.f, 0.5f, 0.f, 0.f);
	//	mesh->SetTransform(t);
	//	mesh->Render();

	//} // 괄호는 복붙을 편리하게 하려고 

	//{
	//	Transform t;
	//	t.offset = Vec4(0.f, 0.f, 0.f, 1.f);
	//	mesh->SetTransform(t);
	//	mesh->Render();

	//} // 괄호는 복붙을 편리하게 하려고 

	//{
	//	Transform t;
	//	t.offset = Vec4(0.f, -1.f, 0.f, 0.f);
	//	mesh->SetTransform(t);
	//	mesh->Render();

	//} // 괄호는 복붙을 편리하게 하려고 

	GEngine->RenderEnd();
}
