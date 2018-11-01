using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Security;

public class RandomCubes : MonoBehaviour {
    public GameObject stationaryPrefab;
    public GameObject movingPrefab;
    public int numberOfCubes = 1000;
    [Range(0, 100)] public int movingPercent = 20;
    private float actualMovingPercent;
    public float movementSpeed = 2f;
    //distribution stuff
    public enum Distribution {Random,Lower,Clustered}
    public Distribution distribution = Distribution.Random;
    public int clusterPointsCount = 5;
    [Range(1f, 25f)] public float clusterRange = 10f;
    [Range(1, 50)]public float lowerRange = 10;
    [Range(1, 25)] public int sizeRange = 5;

    private List<Transform> stationaryTransforms = new List<Transform>();
    private List<Transform> movingTransforms = new List<Transform>();
    private List<Vector3> movingDirections = new List<Vector3>();
    private List<Vector3> clusterPoints = new List<Vector3>();

    private Transform spawnObject(GameObject prefab) {
        GameObject g = Instantiate(prefab, Vector3.zero, Quaternion.identity) as GameObject;
        Transform t = g.transform;
        float randSize = Random.Range(1, sizeRange + 1);
        t.localScale = new Vector3(randSize, randSize, randSize);
        Vector3 randPos = new Vector3();
        switch (distribution) {
            case Distribution.Random:
                randPos = new Vector3(Random.Range(randSize, 100 - randSize), Random.Range(randSize, 100 - randSize), Random.Range(randSize, 100 - randSize));
                break;
            case Distribution.Clustered:
                Vector3 cPoint = clusterPoints[Random.Range(0, clusterPointsCount)];
                randPos = cPoint + new Vector3(Random.Range(-clusterRange + randSize, clusterRange - randSize), Random.Range(-clusterRange + randSize, clusterRange - randSize), Random.Range(-clusterRange + randSize, clusterRange - randSize));
                break;
            case Distribution.Lower:
                randPos = new Vector3(Random.Range(randSize, 100 - randSize), Random.Range(randSize, lowerRange-randSize), Random.Range(randSize, 100 - randSize));
                break;
            default:
                Debug.LogError("selected Distribution not implemented");
                break;
        }

        t.localPosition = randPos;
        t.parent = transform;
        return t;
    }

    private Vector3 randDir() {
        return new Vector3(Random.Range(-1f, 1f), Random.Range(-1f, 1f),
            Random.Range(-1f, 1f)).normalized;
    }

    // Use this for initialization
	void Awake () {

	    for (int i = 0; i < clusterPointsCount; i++) {
            clusterPoints.Add(new Vector3(Random.Range(clusterRange + sizeRange, 100 - clusterRange - sizeRange), Random.Range(clusterRange + sizeRange, 100 - clusterRange - sizeRange), Random.Range(clusterRange + sizeRange, 100 - clusterRange - sizeRange)));
	    }

	    actualMovingPercent = movingPercent/100f;
        //moving
        for (int i = 0; i < numberOfCubes * actualMovingPercent; i++) {
	        movingTransforms.Add(spawnObject(movingPrefab));
            movingDirections.Add(randDir());
	    }
        //stationary
        for (int i = (int)(numberOfCubes * actualMovingPercent); i < numberOfCubes; i++) {
            stationaryTransforms.Add(spawnObject(stationaryPrefab));
        }
	    StartCoroutine(updateDirections());

	}

    private IEnumerator updateDirections() {
        while (true) {
            yield return new WaitForSeconds(Random.Range(0.3f, 2f));
            for (int i = 0; i < movingDirections.Count; i++) {
                movingDirections[i] = randDir();
            }
        }
    }

    private Vector3 clampedPos(Vector3 pos,Vector3 siz) {
        pos = new Vector3(Mathf.Clamp(pos.x, siz.x, 100 - siz.x), Mathf.Clamp(pos.y, siz.y, 100 - siz.y), Mathf.Clamp(pos.z, siz.z, 100 - siz.z));
        return pos;
    }

    // Update is called once per frame
	void Update () {
	    for(int i = 0;i < movingTransforms.Count;i++) {
	        Transform t = movingTransforms[i];
            Vector3 pos = t.localPosition;
	        pos += movingDirections[i]*movementSpeed*Time.deltaTime;
	        t.localPosition = clampedPos(pos, t.localScale);
	    }
	
	}
}
