using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

public class TrackChildren : MonoBehaviour {

	public int frames = 100;
	List<Transform> children = new List<Transform>();
	List<List<Vector3>> savedPositions = new List<List<Vector3>>();
	List<Pair> startInfo = new List<Pair>();

	private int frameCounter = 0;
	bool running = true;

    public string filename = "jsontest.txt";

	// Use this for initialization
	void Start () {
		foreach(Transform child in transform){
			children.Add(child);
			
		}
	    for (int j = 0; j < frames; j++) {
            savedPositions.Add(new List<Vector3>());
	    }
	    //spara startpos och scale på varje child
		for(int i = 0; i < children.Count;i++){
			startInfo.Add(new Pair(children[i].localPosition,children[i].localScale));
		}

	}

	void FixedUpdate () {
		if(running){
			if(frameCounter < frames){
				for(int i = 0; i < children.Count;i++){
                    savedPositions[frameCounter].Add(children[i].localPosition);
				}
				frameCounter++;
			}else{
				running = false;
				SimulationComplete();
			}
		}
	}

    public static JObject getVecJson(Vector3 v) {
        JObject obj = new JObject();

        obj["x"] = new JValue(v.x);
        obj["y"] = new JValue(v.y);
        obj["z"] = new JValue(v.z);
        return obj;
    }

    private JArray getStartInfo() {
        JArray arr = new JArray();
        foreach (Pair p in startInfo) {
            arr.Add(p.getJson());
        }
        return arr;
    }

    public bool compareVec(Vector3 v1, Vector3 v2) {
        return (v1 - v2).sqrMagnitude < 0.001;
    }

    private JArray getFrameData() {
        JArray arr = new JArray();
        for(int j = 0;j<savedPositions.Count;j++) {
            List<Vector3> frame = savedPositions[j];
            
            JArray framearArray = new JArray();
            for (int i = 0; i < frame.Count;i++) {
                if (j > 0) {
                    List<Vector3> lastframe = savedPositions[j - 1];
                    if (!compareVec(frame[i],lastframe[i])) {
                        framearArray.Add(getFrameDataJson(i, frame[i]));
                    }
                }else {
                    framearArray.Add(getFrameDataJson(i, frame[i]));
                }
            }

            arr.Add(framearArray);
        }
        return arr;
    }

    void SimulationComplete() {
	    JObject obj = new JObject();
        obj["StartInfo"] = getStartInfo();
        obj["FrameData"] = getFrameData();
        //writetofile
        System.IO.File.WriteAllText(filename, obj.ToString());
        Debug.Log("Complete!");
        

	}

	[System.Serializable]
	class Pair{
		Vector3 first;
		Vector3 second;
		public Pair(Vector3 fir,Vector3 sec) {
		    first = fir;
		    second = sec;
		}

        public JObject getJson() {
            JObject obj = new JObject();
            obj["position"] = getVecJson(first);
            obj["size"] = getVecJson(second);
            return obj;
        }
	}

    [System.Serializable]
    private class ObjectData {
        int id;
        Vector3 pos;
    }

    private JObject getFrameDataJson(int id, Vector3 v) {
        JObject obj = new JObject();
        obj["id"] = new JValue(id);
        obj["position"] = getVecJson(v);
        return obj;
    }


    /*
    [System.Serializable]
	class Vector {
		float _x,_y,_z;
		public Vector(float x,float y,float z){
			_x = x;
			_y = y;
			_z = z;
		}
		public Vector(Vector3 vec){
			_x = vec.x;
			_y = vec.y;
			_z = vec.z;
		}
		public override string ToString(){
			return ":D";
		}

        public bool compare(Vector v) {
            float pres = 0.01f;
            Debug.Log(Mathf.Abs(v._x - _x));
            //return Mathf.Abs(v._x - _x) < pres && Mathf.Abs(v._y - _y) < pres && Mathf.Abs(v._z - _z) < pres;
            return 
        }

        public JObject getJson() {
            JObject obj = new JObject();
            
	        obj["x"] = new JValue(_x);
            obj["y"] = new JValue(_y);
            obj["z"] = new JValue(_z);
            return obj;
	    }
	}*/

}
