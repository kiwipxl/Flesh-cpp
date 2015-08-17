/*
Gets all data from ferr2D terrain objects in Unity and writes the data in a specific format 
to a file that can be read by my custom terrain loader
*/

using UnityEngine;
using System.Collections;
using System.IO;

public class Ferr2DExporter : MonoBehaviour {

    bool export_complete = false;

	void Start() {
        
    }

	void Update() {
        //exporter is put in update due to some issues where all components aren't loaded on
        //terrain by the time start is called above
        if (export_complete) return;

        Debug.Log("exporter running...");

        int index = 1;
        string data = "-- ferr2D export data --\n\n";
        string terrain_name = "terrain";
        string terrain_name_full;
        while (true) {
            terrain_name_full = terrain_name + index;
            GameObject obj = GameObject.Find(terrain_name_full);
            if (obj == null) break;

            data += "-- TERRAIN_BEGIN --\n";

            Mesh m = obj.GetComponent<MeshFilter>().mesh;
            Vector3[] vertices = m.vertices;

            string vert_str = "";
            string indi_str = "";
            for (int n = 0; n < vertices.Length; ++n) {
                vert_str += vertices[n].x + "," + vertices[n].y + ",";
            }
            for (int n = 0; n < m.subMeshCount; ++n) {
                int[] indices = m.GetIndices(n);
                for (int i = 0; i < indices.Length; ++i) {
                    indi_str += indices[i] + ",";
                }
            }

            string uv_str = "";
            for (int n = 0; n < m.uv.Length; ++n) {
                uv_str += m.uv[n].x + "," + m.uv[n].y + ",";
            }

            string collider_str = "";
            PolygonCollider2D poly_col = obj.GetComponent<PolygonCollider2D>();
            if (poly_col != null) {
                Vector2[] points = poly_col.points;
                for (int n = 0; n < points.Length; ++n) {
                    collider_str += points[n].x + "," + points[n].y + ",";
                }
            }else {
                Debug.Log("warning: no collider attached to terrain (" + terrain_name_full + ")");
            }

            Ferr2DT_PathTerrain path_terrain = obj.GetComponent<Ferr2DT_PathTerrain>();
            string vertex_colour_str = "";
            if (path_terrain != null) {
                vertex_colour_str += path_terrain.vertexColor.r + "," +
                                     path_terrain.vertexColor.g + "," +
                                     path_terrain.vertexColor.b + "," +
                                     path_terrain.vertexColor.a + ",";
            }
            else {
                Debug.Log("error: ferr2d path terrain component must be attached (" + terrain_name_full + ")");
            }

            string temp = "";
            temp += "vertex_data:" + vert_str + "\n\n";
            temp += "indices:" + indi_str + "\n\n";
            temp += "uvs:" + uv_str + "\n\n";
            if (collider_str != "") temp += "collider_points:" + collider_str + "\n\n";
            temp += "-- attribs --\n\n";
            temp += "name:" + terrain_name_full + "\n";
            temp += "pos:" + obj.transform.position.x + "," + obj.transform.position.y + ",\n";
            temp += "fill_indices:0," + m.GetIndices(0).Length + ",\n";
            temp += "edge_indices:" + m.GetIndices(0).Length + "," + m.GetIndices(1).Length + ",\n";
            if (vertex_colour_str != "") temp += "vertex_colour:" + vertex_colour_str + "\n\n";
			
            data += temp;
            data += "-- TERRAIN_END --\n\n";

            Debug.Log("(" + terrain_name_full + ") data get successful");

            index += 1;
        }

        string spawn_points_str = "";
        int s = 1;
        while (true) {
            GameObject spawn_obj = GameObject.Find("spawnpoint" + s);
            if (spawn_obj == null) break;
            spawn_points_str += spawn_obj.transform.position.x + "," + spawn_obj.transform.position.y + ",";
            ++s;
        }
        if (spawn_points_str != "") data += "global_spawn_points:" + spawn_points_str + "\n\n";

        if (index == 1) {
            Debug.Log("terrain object (" + terrain_name_full + ") could not be found. Exporter requires at least 1 terrain object");
        }else {
            string file_name = "terrain.f2d";
            StreamWriter file = new StreamWriter(file_name);
            file.Write(data);
            file.Close();
            Debug.Log("data wrote to file: " + file_name);
        }

        export_complete = true;
	}
}
