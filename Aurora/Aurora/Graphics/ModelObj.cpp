#include <Aurora/Graphics/ModelObj.h>
#include <Aurora/Graphics/TextureManager.h>

namespace Aurora
{
	namespace Graphics
	{

		void ObjModel::LoadMaterials(const char *Filname)
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

							//newMaterial->colorMapFilename =  "Assets/Obj/";
							newMaterial->colorMapFilename = buffer;

							std::string pathName(Filname);
							size_t found = pathName.find_last_of("/");
							pathName = pathName.substr(0,found)+ "/" + buffer;

//							TextureManager::Instance()->LoadTextureFromFile(pathName);
//							newMaterial->texturID = TextureManager::Instance()->GetTextureNumber(pathName);

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

//							TextureManager::Instance()->LoadTextureFromFile(pathName);
//							newMaterial->lightMapID = TextureManager::Instance()->GetTextureNumber(pathName);
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

		void ObjModel::LoadObj(const char *FileName)
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
		}

		void ObjModel::FindLightMaps()
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

//						TextureManager::Instance()->LoadTextureFromFile(pathName);
						int lightMap;// = TextureManager::Instance()->GetTextureNumber(pathName);

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

//					TextureManager::Instance()->LoadTextureFromFile(pathName);
					int lightMap;//= TextureManager::Instance()->GetTextureNumber(pathName);

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

		void ObjModel::Optimize2()
		{
			for (unsigned int i = 0; i < mMeshes.size();i++)
			{
				ObjMesh *mesh = mMeshes[i];

				std::vector<TexturesPSPVertex> temp;
				std::vector<int> tempIndices;

				for(unsigned int f = 0;f < mesh->mFace.size();f++)
				{
					//create temp vertex
					TexturesPSPVertex vert;
					vert.u = allUVMap[mesh->mUVFace[f].x].u;
					vert.v = allUVMap[mesh->mUVFace[f].x].v;
					vert.x = allVertex[mesh->mFace[f].x].x;
					vert.y = allVertex[mesh->mFace[f].x].y;
					vert.z = allVertex[mesh->mFace[f].x].z;

					//check if exist
					bool found = false;
					for(unsigned int t = 0;t < temp.size();t++)
					{
						if(temp[t].u == vert.u && temp[t].v == vert.v && temp[t].x == vert.x && temp[t].y == vert.y && temp[t].z == vert.z)
						{
							found = true;
							tempIndices.push_back(t);
							break;
						}
					}

					if (found == false)
					{
						mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].x]);
						temp.push_back(vert);
						tempIndices.push_back(temp.size()-1);
					}

					//////////////////////////////////////////////////////////////////////////
					vert.u = allUVMap[mesh->mUVFace[f].y].u;
					vert.v = allUVMap[mesh->mUVFace[f].y].v;
					vert.x = allVertex[mesh->mFace[f].y].x;
					vert.y = allVertex[mesh->mFace[f].y].y;
					vert.z = allVertex[mesh->mFace[f].y].z;

					//check if exist
					found = false;
					for(unsigned int t = 0;t < temp.size();t++)
					{
						if(temp[t].u == vert.u && temp[t].v == vert.v && temp[t].x == vert.x && temp[t].y == vert.y && temp[t].z == vert.z)
						{
							found = true;
							tempIndices.push_back(t);
							break;
						}
					}

					if (found == false)
					{
						mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].y]);
						temp.push_back(vert);
						tempIndices.push_back(temp.size()-1);
					}

					//////////////////////////////////////////////////////////////////////////
					vert.u = allUVMap[mesh->mUVFace[f].z].u;
					vert.v = allUVMap[mesh->mUVFace[f].z].v;
					vert.x = allVertex[mesh->mFace[f].z].x;
					vert.y = allVertex[mesh->mFace[f].z].y;
					vert.z = allVertex[mesh->mFace[f].z].z;

					//check if exist
					found = false;
					for(unsigned int t = 0;t < temp.size();t++)
					{
						if(temp[t].u == vert.u && temp[t].v == vert.v && temp[t].x == vert.x && temp[t].y == vert.y && temp[t].z == vert.z)
						{
							found = true;
							tempIndices.push_back(t);
							break;
						}
					}

					if (found == false)
					{
						mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].z]);
						temp.push_back(vert);
						tempIndices.push_back(temp.size()-1);
					}
				}

				//copy temp vertices and indieces
				mesh->meshVertices = new TexturesPSPVertex[temp.size()];
				for(int v = 0;v < temp.size();v++)
				{
					memcpy(&mesh->meshVertices[v],&temp[v],sizeof(TexturesPSPVertex));
				}

				mesh->indices = new int[tempIndices.size()];
				for(int v = 0;v < tempIndices.size();v++)
				{
					memcpy(&mesh->indices[v],&tempIndices[v],sizeof(int));
				}

				mesh->triangleCount = mesh->mFace.size();
				mesh->vertexCount = temp.size();
				mesh->indicesCount = tempIndices.size();

				//try optimize indieces for cashe misses
				/*VertexCache vertex_cache;
				VertexCacheOptimizer vco;

				int misses = vertex_cache.GetCacheMissCount(mesh->indices,mesh->triangleCount);

				VertexCacheOptimizer::Result res = vco.Optimize(mesh->indices,mesh->triangleCount);
				int misses2 = vertex_cache.GetCacheMissCount(mesh->indices,mesh->triangleCount);*/

				//create triangle strips :D
				/*STRIPERCREATE sc;

				sc.DFaces			= (unsigned int*)mesh->indices;
				sc.NbFaces			= mesh->triangleCount;
				sc.AskForWords		= true;
				sc.ConnectAllStrips	= true;
				sc.OneSided			= true;
				sc.SGIAlgorithm		= true;

				Striper Strip;
				Strip.Init(sc);

				STRIPERRESULT sr;
				Strip.Compute(sr);

				std::vector<unsigned int> testor;

				uword* Refs = (uword*)sr.StripRuns;
				for(udword i=0;i<sr.NbStrips;i++)
				{
					udword NbRefs = sr.StripLengths[i];
					for(udword j=0;j<NbRefs;j++)
					{
						//fprintf(stdout, "%d ", *Refs++);
						testor.push_back(*Refs++);
					}
				}

				delete [] mesh->indices;

				mesh->indices = new int[testor.size()];
				for(int v = 0;v < testor.size();v++)
				{
					memcpy(&mesh->indices[v],&testor[v],sizeof(int));
				}

				mesh->indicesCount = testor.size();*/

				//end

				temp.clear();
				tempIndices.clear();

				mesh->mUVFace.clear();
				mesh->mFace.clear();
				mesh->mNormalFace.clear();

			}

			allVertex.clear();
			allNormal.clear();
			allUVMap.clear();

		}

		void ObjModel::Optimize()
		{
			for (unsigned int i = 0; i < mMeshes.size();i++)
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
			allUVMap.clear();
		}

		void  ObjModel::Render2()
		{
			/*for (unsigned int i = 0; i < mMeshes.size();i++)
			{
				ObjMesh *mesh = mMeshes[i];

				glColor3f(1,1,1);

				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, GLsizei(sizeof(TexturesPSPVertex)), &mesh->meshVertices[0].x);
				
				if(mesh->mMaterial != -1)
				{
					//texture
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glActiveTexture(GL_TEXTURE0);
					glClientActiveTexture(GL_TEXTURE0);

					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, mMaterials[mesh->mMaterial]->texturID);
					glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(TexturesPSPVertex)), &mesh->meshVertices[0].u);
				}
		

				if (mesh->mMaterial != -1 && mMaterials[mesh->mMaterial]->lightmapping)
				{
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glActiveTexture(GL_TEXTURE1);
					glClientActiveTexture(GL_TEXTURE1);

					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, mMaterials[mesh->mMaterial]->lightMapID);
					glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(TexturesPSPVertex)), &mesh->meshVertices[0].u);

					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				}

				//glDrawElements(GL_TRIANGLE_STRIP, GLsizei(mesh->indicesCount), GL_UNSIGNED_INT, &mesh->indices[0]);
				glDrawElements(GL_TRIANGLES, GLsizei(mesh->indicesCount), GL_UNSIGNED_INT, &mesh->indices[0]);

				glDisable(GL_TEXTURE_2D);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);

				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			}*/
		}

		void ObjModel::SaveOptimized(const char *FileName)
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

		void ObjModel::LoadOptimized(const char *FileName)
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
