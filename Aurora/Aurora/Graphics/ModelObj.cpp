#include <Aurora/Graphics/ModelObj.h>
#include <Aurora/Graphics/TextureManager.h>
#include <Aurora/Graphics/RenderManager.h>

namespace Aurora
{
	namespace Graphics
	{

		void ModelObj::LoadMaterials(const char *Filname)
		{
			FILE *fp = fopen(Filname,"r");

			if (!fp)
			{
				return;
			}

			ObjMaterial *newMaterial = new ObjMaterial();

			int illum = 0;
			char buffer[255] = {0};

			bool newMat = false;

			while (fscanf(fp, "%s", buffer) != EOF)
			{
				switch(buffer[0])
				{
					case 'n': //new material
					{
						if (newMat)
						{
							mMaterials.push_back(newMaterial);
							newMaterial = new ObjMaterial();
							newMat = false;
						}

						fgets(buffer, sizeof(buffer), fp);
						sscanf(buffer, "%s %s", buffer, buffer);

						newMaterial->name = buffer;
					}
					break;

					case 'N': //Ns - shiness
					{
							fscanf(fp, "%f", &newMaterial->shininess);

							newMaterial->shininess /= 1000.0f;
					}
					break;

					case 'K':
					{
						switch(buffer[1])
						{
							case 'a'://Ka - ambient
							{
								fscanf(fp, "%f %f %f",&newMaterial->ambient[0],&newMaterial->ambient[1],&newMaterial->ambient[2]);
								newMaterial->ambient[3] = 1.0f;
							}
							break;
							case 'd'://Kd - diffuse
							{
								fscanf(fp, "%f %f %f",&newMaterial->diffuse[0],&newMaterial->diffuse[1],&newMaterial->diffuse[2]);
								newMaterial->diffuse[3] = 1.0f;
							}
							break;
							case 's'://Ks - specular
							{
								fscanf(fp, "%f %f %f",&newMaterial->specular[0],&newMaterial->specular[1],&newMaterial->specular[2]);
								newMaterial->specular[3] = 1.0f;
							}
							break;
							default:
							{
								fgets(buffer, sizeof(buffer), fp);
							}
							break;
						}
					}
					break;

					case 'T': // Tr
					{
						switch (buffer[1])
						{
							case 'r': // Tr
							{
								fscanf(fp, "%f", &newMaterial->alpha);
								newMaterial->alpha = 1.0f - newMaterial->alpha;
							}
							break;

							default:
							{
								fgets(buffer, sizeof(buffer), fp);
							}
							break;
						}
					}
					break;

					case 'd':
					{
						fscanf(fp, "%f", &newMaterial->alpha);
					}
					break;

					case 'i': // illum
					{
						fscanf(fp, "%d", &illum);

						if (illum == 1)
						{
							newMaterial->specular[0] = 0.0f;
							newMaterial->specular[1] = 0.0f;
							newMaterial->specular[2] = 0.0f;
							newMaterial->specular[3] = 1.0f;
						}
					}
					break;

					case 'm': // map_Kd, map_bump
					{
						if (strstr(buffer, "map_Kd") != 0)
						{
							fgets(buffer, sizeof(buffer), fp);
							sscanf(buffer, "%s %s", buffer, buffer);

							//newMaterial->colorMapFilename = buffer;

							std::string pathName(Filname);
							size_t found = pathName.find_last_of("/");
							pathName = pathName.substr(0,found)+ "/" + buffer;

							newMaterial->texturID  = TextureManager::Instance()->loadImageFromFile(pathName)->_id;
							newMaterial->colorMapFilename = pathName;

							newMat = true;

						}
						else if (strstr(buffer, "map_bump") != 0)
						{
							/*fgets(buffer, sizeof(buffer), fp);
							sscanf(buffer, "%s %s", buffer, buffer);

							newMaterial->bumpMapFilename =  "Assets/Obj/";
							newMaterial->bumpMapFilename += buffer;

							TextureManager *tManager = TextureManager::Instance();

							if (!tManager->TextureExist(newMaterial->bumpMapFilename))
							{
								tManager->LoadTexture(newMaterial->bumpMapFilename);
							}

							newMaterial->normaltextureID = tManager->GetTextureNumber(newMaterial->bumpMapFilename);
							newMat = true;*/
						}else if(strstr(buffer, "map_Ke") != 0)
						{
							fgets(buffer, sizeof(buffer), fp);
							sscanf(buffer, "%s %s", buffer, buffer);

							std::string pathName(Filname);
							size_t found = pathName.find_last_of("/");
							pathName = pathName.substr(0,found)+ "/" + buffer;

							newMaterial->lightMapID  = TextureManager::Instance()->loadImageFromFile(pathName)->_id;
							newMaterial->lightmapping = true;
						}
						else
						{
							fgets(buffer, sizeof(buffer), fp);
						}
					}
					break;

					default:
						fgets(buffer, sizeof(buffer), fp);
					break;
				}
			}


			mMaterials.push_back(newMaterial);
			fclose(fp);
		}

		void ModelObj::LoadObj(const char *FileName)
		{
			FILE *fp = fopen(FileName,"r");

			if (!fp)
			{
				return;
			}

			pathName = FileName;

			ObjMesh *newMesh = new ObjMesh();

			char ch				= 0;
			char ch2			= 0;
			char strLine[255]	= {0};
			char buff[512];

			bool faceLoad = false;
			bool uvFace = false;
			bool normal = false;

			while(!feof(fp))
			{
				//get first character
				ch = fgetc(fp);

				switch(ch)
				{
				case 'm': //material info
					{
						char text[100];
						char materialName[100];

						//read material info
						fscanf(fp,"%s %s", &text, &materialName);

						std::string pathName(FileName);
						size_t found = pathName.find_last_of("/");
						pathName = pathName.substr(0,found) + "/" + materialName;

						LoadMaterials(pathName.c_str());

						//read line to the end
						fgets(strLine, 100, fp);
					}
					break;

				case 'u'://use material
				{
					if (faceLoad)
					{
						mMeshes.push_back(newMesh);
						meshCount++;

						//new newmesh
						newMesh = new ObjMesh();

						faceLoad = false;
						uvFace = false;
						normal = false;
					}

					char name[50];
					char name2[50];

					//read material name
					fscanf(fp,"%s %s", &name, &name2);

					newMesh->mMaterial = 0;

					for(unsigned int i = 0;i < mMaterials.size();i++)
					{
						if(strcmp(mMaterials[i]->name.c_str(),name2) == 0)
						{
							newMesh->mMaterial = i;
						}
					}

					//read line to the end
					fgets(strLine, 100, fp);
				}
				break;

				case 'g': //mesh name
					{
						//read mesh name
						fscanf(fp," %s", newMesh->name);

						//read line to the end
						fgets(strLine, 100, fp);
					}
					break;



				case 'v': //vertex found
					{
						//get second character
						ch2 = fgetc(fp);

						switch(ch2)
						{
						case ' ':
							{
								if (faceLoad)
								{
									mMeshes.push_back(newMesh);
									meshCount++;

									//new newmesh
									newMesh = new ObjMesh();

									faceLoad = false;
									uvFace = false;
									normal = false;
								}

								//new vertex position
								Vector3 vPosition;

								//read vertex info
								fscanf(fp,"%f %f %f", &vPosition.x, &vPosition.y, &vPosition.z);

								allVertex.push_back(vPosition);

								//read line to the end
								fgets(strLine, 100, fp);
							}
							break;

						case 't': //texture info
							{
								uvFace = true;

								//new uv map
								ObjUV nObjUV;

								//read uv map
								fscanf(fp,"%f %f", &nObjUV.u, &nObjUV.v);

								allUVMap.push_back(nObjUV);

								//read line to the end
								fgets(strLine, 100, fp);
							}
							break;

						case 'n': //normal info
							{
								normal = true;

								//new normal position
								Vector3 nNormal;

								//read normal
								fscanf(fp,"%f %f %f", &nNormal.x, &nNormal.y ,&nNormal.z);

								allNormal.push_back(nNormal);

								//read line to the end
								fgets(strLine, 100, fp);
							}
							break;

						default:
							break;
						}
					}
					break;

				case 'f': //face info
					{
						faceLoad = true;

						//new face
						ObjFace nFace;
						ObjFace nUVFace;
						ObjFace nNorm;

						/* Read whole line */
						fgets (buff, sizeof (buff), fp);

						if (sscanf(buff,"%d/%d/%d %d/%d/%d %d/%d/%d",&nFace.x,&nUVFace.x,&nNorm.x,&nFace.y,&nUVFace.y,&nNorm.y,&nFace.z,&nUVFace.z,&nNorm.z) == 9)
						{
							//push UV face to mesh
							nUVFace.x -=1;nUVFace.y -=1;nUVFace.z -=1;

							newMesh->mUVFace.push_back(nUVFace);

							//we must -1 to all indicies
							nNorm.x -=1;nNorm.y -=1;nNorm.z -=1;

							//push
							newMesh->mNormalFace.push_back(nNorm);
							uvFace = true;
							normal = true;
						}else
						if (sscanf(buff,"%d/%d %d/%d %d/%d",&nFace.x,&nUVFace.x,&nFace.y,&nUVFace.y,&nFace.z,&nUVFace.z) == 6)
						{
							//push UV face to mesh
							nUVFace.x -=1;nUVFace.y -=1;nUVFace.z -=1;
							newMesh->mUVFace.push_back(nUVFace);
							uvFace = true;
						}else
						{
							sscanf(buff,"%d %d %d",&nFace.x,&nFace.y,&nFace.z);
						}

						//we must -1 to all indicies
						nFace.x -=1;nFace.y -=1;nFace.z -=1;

						//push face to mesh
						newMesh->mFace.push_back(nFace);
					}
					break;

				case '\n':

					break;

				default:
					//read line to the end
					fgets(strLine, 100, fp);
					break;
				}
			}

			//ad last submesh or if thhere is only one
			mMeshes.push_back(newMesh);

			fclose(fp);

			//now build vertices in proper way for each platform
			RenderManager::Instance()->_createModelObjVertices(this);
		}

		void ModelObj::FindLightMaps()
		{
			for (unsigned int i = 0; i < mMeshes.size();i++)
			{
				ObjMesh *mesh = mMeshes[i];

				//for each mesh we need to check if there is material
				if (mesh->mMaterial != -1)
				{
					//there is material :) so we are in half way
					//now check if there is already lightmap
					if (mMaterials[mesh->mMaterial]->lightmapping)
					{
						//everything is ok - we don't do anything
					}else
					{
						//lest see if there is a lightmap with the same name as object
						size_t found = pathName.find_last_of("/");
						pathName = pathName.substr(0,found) + "/" + mesh->name + "LightingMap.png";

						int lightMap = TextureManager::Instance()->loadImageFromFile(pathName)->_id;

						if (lightMap != -1)
						{
							mMaterials[mesh->mMaterial]->lightmapping = true;
							mMaterials[mesh->mMaterial]->lightMapID = lightMap;
						}
					}
				}else
				{
					//if there is no material then make one :>
					ObjMaterial *newMaterial = new ObjMaterial();

					//lest see if there is a lightmap with the same name as object
					size_t found = pathName.find_last_of("/");
					pathName = pathName.substr(0,found) + "/" + mesh->name + "LightingMap.png";
										
					int lightMap = TextureManager::Instance()->loadImageFromFile(pathName)->_id;

					if (lightMap != -1)
					{
						//set it as a main texture
						newMaterial->texturID = lightMap;

						//add to container
						mMaterials.push_back(newMaterial);
						//assign this material to mesh
						mesh->mMaterial = mMaterials.size() - 1;
					}
				}
			}
		}

		void ModelObj::Optimize()
		{
			/*for (unsigned int i = 0; i < mMeshes.size();i++)
			{
				ObjMesh *mesh = mMeshes[i];

				//build mesh vertices
				mesh->meshVertices = new TexturesPSPVertex[mesh->mFace.size() * 3];
				mesh->triangleCount = mesh->mFace.size();
				mesh->vertexCount = mesh->mFace.size() * 3;

				mesh->indices = new int[mesh->mFace.size() * 3];

				int j =0;

				for(unsigned int f = 0;f < mesh->mFace.size();f++)
				{
					TexturesPSPVertex vert1;
					vert1.u = allUVMap[mesh->mUVFace[f].x].u;// * 32767;//32767 127
					vert1.v = allUVMap[mesh->mUVFace[f].x].v;// * 32767;;
					vert1.x = allVertex[mesh->mFace[f].x].x;// * 32767;
					vert1.y = allVertex[mesh->mFace[f].x].y;// * 32767;
					vert1.z = allVertex[mesh->mFace[f].x].z;// * 32767;
					memcpy(&mesh->meshVertices[j],&vert1,sizeof(TexturesPSPVertex));

					mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].x]);
					j++;

					TexturesPSPVertex vert2;
					vert2.u = allUVMap[mesh->mUVFace[f].y].u;//  * 32767;
					vert2.v = allUVMap[mesh->mUVFace[f].y].v;//  * 32767;
					vert2.x = allVertex[mesh->mFace[f].y].x;// * 32767;
					vert2.y = allVertex[mesh->mFace[f].y].y;// * 32767;
					vert2.z = allVertex[mesh->mFace[f].y].z;//  * 32767;
					memcpy(&mesh->meshVertices[j],&vert2,sizeof(TexturesPSPVertex));

					mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].y]);
					j++;

					TexturesPSPVertex vert3;
					vert3.u = allUVMap[mesh->mUVFace[f].z].u;//  * 32767;
					vert3.v = allUVMap[mesh->mUVFace[f].z].v;//  * 32767;
					vert3.x = allVertex[mesh->mFace[f].z].x;//  * 32767;
					vert3.y = allVertex[mesh->mFace[f].z].y;//  * 32767;
					vert3.z = allVertex[mesh->mFace[f].z].z;//  * 32767;
					memcpy(&mesh->meshVertices[j],&vert3,sizeof(TexturesPSPVertex));

					mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].z]);
					j++;

				}

				mesh->mUVFace.clear();
				mesh->mFace.clear();
				mesh->mNormalFace.clear();
			}

			allVertex.clear();
			allNormal.clear();
			allUVMap.clear();*/
		}

		void ModelObj::SaveOptimized(const char *FileName)
		{
			FILE *binaryFile;
			binaryFile = fopen (FileName, "wb");
			if (!binaryFile)
			{
				fprintf (stderr, "Error: couldn't open \"%s\"!\n", FileName);
				return ;
			}

			//materials count
			int count = mMaterials.size();
			fwrite(&count,sizeof(int),1,binaryFile);

			//write all materials
			for(unsigned int i = 0; i < mMaterials.size();i++)
			{
				char name[50];
				sprintf(name,"%s", mMaterials[i]->colorMapFilename.c_str());
				fwrite(name,sizeof(name),1,binaryFile);
			}

			//mesh count
			count = mMeshes.size();
			fwrite(&count,sizeof(int),1,binaryFile);

			//write every mesh
			for (unsigned int i = 0; i < mMeshes.size();i++)
			{
				ObjMesh *mesh = mMeshes[i];

				//material number
				fwrite(&mesh->mMaterial,sizeof(int),1,binaryFile);

				//aabb min
				fwrite(&mesh->aabb.min,sizeof(Vector3),1,binaryFile);

				//aabb max
				fwrite(&mesh->aabb.max,sizeof(Vector3),1,binaryFile);

				//triangle count
				//fwrite(&mesh->triangleCount,sizeof(int),1,binaryFile);

				//vertices count
				fwrite(&mesh->vertexCount,sizeof(int),1,binaryFile);

				//indices count
				fwrite(&mesh->indicesCount,sizeof(int),1,binaryFile);

				//write all vertices
				fwrite(mesh->meshVertices,sizeof(TexturesPSPVertex),mesh->vertexCount,binaryFile);

				int *test = new int[mesh->indicesCount];
				for(int d = 0;d< mesh->indicesCount ;d++)
				{
					test[d] = mesh->indices[d];
				}

				//write all indieces
				fwrite(test,sizeof(int),mesh->indicesCount,binaryFile);
			}

			fclose(binaryFile);
		}

		void ModelObj::LoadOptimized(const char *FileName)
		{
			FILE *binaryFile;
			binaryFile = fopen (FileName, "rb");
			if (!binaryFile)
			{
				fprintf (stderr, "Error: couldn't open \"%s\"!\n", FileName);
				return ;
			}


			//materials count
			int count = 0;
			fread(&count,sizeof(int),1,binaryFile);

			//read all materials
			for(int i = 0; i < count;i++)
			{
				char textureName[50];
				fread(textureName,sizeof(textureName),1,binaryFile);

				ObjMaterial *newMaterial = new ObjMaterial();

				newMaterial->colorMapFilename = textureName;

				std::string pathName(FileName);
				size_t found = pathName.find_last_of("/");
				pathName = pathName.substr(0,found)+ "/" + textureName;

//				TextureManager::Instance()->LoadTextureFromFile(pathName);
//				newMaterial->texturID = TextureManager::Instance()->GetTextureNumber(pathName);

				mMaterials.push_back(newMaterial);
			}

			//meshes
			count = 0;
			fread(&count,sizeof(int),1,binaryFile);

			//read all meshes
			for(int i = 0; i < count;i++)
			{
				ObjMesh *mesh = new ObjMesh();

				//material number
				fread(&mesh->mMaterial,sizeof(int),1,binaryFile);

				//aabb min
				fread(&mesh->aabb.min,sizeof(Vector3),1,binaryFile);

				//aabb max
				fread(&mesh->aabb.max,sizeof(Vector3),1,binaryFile);

				//triangle count
				//fread(&mesh->triangleCount,sizeof(int),1,binaryFile);

				//vertices count
				fread(&mesh->vertexCount,sizeof(int),1,binaryFile);

				//indices count
				fread(&mesh->indicesCount,sizeof(int),1,binaryFile);

				//write all vertices
				mesh->meshVertices = new TexturesPSPVertex[mesh->vertexCount];
				fread(mesh->meshVertices,sizeof(TexturesPSPVertex),mesh->vertexCount,binaryFile);

				//write all indieces
				mesh->indices = new int[mesh->indicesCount];
				int *test = new int[mesh->indicesCount];
				fread(test,sizeof(int),mesh->indicesCount,binaryFile);
		
				for(int d = 0;d< mesh->indicesCount ;d++)
				{
					mesh->indices[d] = test[d];
				}

		
				//fread(mesh->indices,sizeof(int),mesh->indicesCount,binaryFile);

				mMeshes.push_back(mesh);
			}


			fclose(binaryFile);
		}
	}
}
