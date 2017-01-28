#include "Game/Raycast3D.hpp"

bool Raycast3DToPoint(const Vector3& start, const Vector3& end, RaycastResult* out_result) {
	Vector3 displacement = end - start;
	Vector3 current = start;
	Vector3 step = displacement * 0.01f;

	for (int i = 0; i < 200; i++) {
		Block* block = g_theWorld->GetBlockForWorldCoords(current);


		if (block != nullptr) {
			if (block->GetBlockType() != BLOCK_AIR) {

				if (out_result != NULL) {
					out_result->m_didImpact = true;
					out_result->m_impactPos = current;
					out_result->m_impactBlock = block;

					//Compute surface normal
					float x = floor(current.x) + 0.5f;
					float y = floor(current.y) + 0.5f;
					float z = floor(current.z) + 0.5f;

					WorldPosition middleOfBlock = WorldPosition(x, y, z);

					Vector3 NorthFaceNormal = Vector3(-1.f, 0.f, 0.f);
					Vector3 SouthFaceNormal = Vector3(1.f, 0.f, 0.f);
					Vector3 EastFaceNormal = Vector3(0.f, 1.f, 0.f);
					Vector3 WestFaceNormal = Vector3(0.f, -1.f, 0.f);
					Vector3 TopFaceNormal = Vector3(0.f, 0.f, 1.f);
					Vector3 BottomFaceNormal = Vector3(0.f, 0.f, -1.f);

					
					//Get last-intersected air block
					Vector3 currBlockCoords = Chunk::GetWorldCoordsForLocalCoords(current);
					Vector3 prevBlockCoords = Chunk::GetWorldCoordsForLocalCoords(current - step);
					Vector3 faceDisp = currBlockCoords - prevBlockCoords;
					out_result->m_prevImpactBlockPos = prevBlockCoords;
					out_result->m_prevImpactPos = current - step;

					if (faceDisp == Vector3(1.f, 0.f, 0.f))
						out_result->m_impactSurfaceNormal = FACE_NORTH;
					else if (faceDisp == Vector3(-1.f, 0.f, 0.f))
						out_result->m_impactSurfaceNormal = FACE_SOUTH;
					else if (faceDisp == Vector3(0.f, 1.f, 0.f))
						out_result->m_impactSurfaceNormal = FACE_WEST;
					else if (faceDisp == Vector3(0.f, -1.f, 0.f))
						out_result->m_impactSurfaceNormal = FACE_EAST;
					else if (faceDisp == Vector3(0.f, 0.f, 1.f))
						out_result->m_impactSurfaceNormal = FACE_BOTTOM;
					else if (faceDisp == Vector3(0.f, 0.f, -1.f))
						out_result->m_impactSurfaceNormal = FACE_TOP;
					else
						out_result->m_impactSurfaceNormal = FACE_NONE;
				}
				return true;
			}
		}
		current += step;
	}

	return false;
}