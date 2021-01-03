package com.ParseObjectPositions;

import org.w3c.dom.*;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.reflect.Array;
import java.util.*;
import java.util.stream.Collectors;

public class Main {

    static public void main(String[] args) throws IOException, ParserConfigurationException, SAXException {
        Properties properties = new Properties();
        properties.load(new FileInputStream(args[0]));
        Set<String> keys = properties.stringPropertyNames();
        List<String> sortedKeys = keys.stream().collect(Collectors.toList());
        Collections.sort(sortedKeys);

        String currentName = null;
        Map<Integer,String> framesToName = new HashMap<>();
        for (String key : sortedKeys) {
            if (key.contains("object.name")) {
                currentName = properties.getProperty(key);
                continue;
            }

            if (key.contains("object.range.frame")) {
                String[] pair = properties.getProperty(key).split("-");
                int rangeStart =Integer.parseInt(pair[0]);
                int rangeEnd =Integer.parseInt(pair[1]);
                for (int i = rangeStart ; i <= rangeEnd ; i++) {
                    framesToName.put(i,currentName);
                }
                continue;
            }
            if (key.contains("object.set.frame")) {
                framesToName.put(Integer.parseInt(properties.getProperty(key)),currentName);
                continue;
            }
        }

        DocumentBuilderFactory factory= DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = factory.newDocumentBuilder();
        Document document=builder.parse(new File(args[1]));
        Element documentElement=document.getDocumentElement();
        String mapData = documentElement.getElementsByTagName("data").item(0).getChildNodes().item(0).getTextContent();
        mapData = mapData.trim();
        String[] tiles = mapData.split(",");
        int maxTileindex = 0;
        for (String tile : tiles) {
            try {
                int index = Integer.parseInt(tile);
                if (index > maxTileindex) {
                    maxTileindex = index;
                }
            } catch (NumberFormatException e) {
            }
        }
        maxTileindex++;
        NodeList objects = documentElement.getElementsByTagName("object");
        List<ActivationObject> activationObjects = new ArrayList<>();

        int maxX = 0;
        int maxY = 0;
        for (int i = 0 ; i < objects.getLength(); i++) {
            Node node = objects.item(i);
            NamedNodeMap attributes = node.getAttributes();
            int id = Integer.parseInt(attributes.getNamedItem("id").getTextContent());
            int gid = Integer.parseInt(attributes.getNamedItem("gid").getTextContent()) - maxTileindex;
            double x = Double.parseDouble(attributes.getNamedItem("x").getTextContent());
            double y = Double.parseDouble(attributes.getNamedItem("y").getTextContent());

            String objectName = framesToName.get(gid);
            if (objectName == null) {
                System.err.println(args[1]+"(1) : Error : Object: " + id + " : Frame index not found: " + gid);
            } else {
                ActivationObject activationObject = new ActivationObject(objectName,x ,y-21);
                if (activationObject.getXpos() > maxX) {
                    maxX = activationObject.getXpos();
                }
                if (activationObject.getYpos() > maxY) {
                    maxY = activationObject.getYpos();
                }
                System.out.println("\t\t+AnimationEnemyMapPlacement ~.placedEnemy" + activationObjects.size()+" , " + objectName + "\t, " + activationObject.getXpos() +" , " + activationObject.getYpos());
                activationObjects.add(activationObject);
            }
        }

        System.out.println();
        System.out.println("; Num: + " + activationObjects.size() + " Max: " + maxX + " , " + maxY);

        TreeMap<Integer,Integer> []rangesX = new TreeMap[(maxX/8)+1];
        TreeMap<Integer,Integer> []rangesY = new TreeMap[(maxY/8)+1];
        for (int i = 0 ; i < activationObjects.size() ; i++) {
            ActivationObject activationObject = activationObjects.get(i);
            int xpos = activationObject.getXpos()/8;
            int ypos = activationObject.getYpos()/8;
            if (rangesX[xpos] == null) {
                rangesX[xpos] = new TreeMap<>();
            }
            if (rangesY[ypos] == null) {
                rangesY[ypos] = new TreeMap<>();
            }
            rangesX[xpos].put(activationObject.getYpos() , i);
            rangesY[ypos].put(activationObject.getXpos() , i);
        }

        // Generate X list
        System.out.println("AnimationEnemyMapPlacementsX");
        int prevIndex = -1;
        for (int i = 0 ; i < rangesX.length ; i++) {
            if (rangesX[i] == null) {
                continue;
            }
            int nextIndex = -1;
            System.out.println(".mapX" + i);
            for (int j = i+1 ; j < rangesX.length ; j++) {
                if (rangesX[j] != null) {
                    nextIndex = j;
                    break;
                }
            }
            String nextText = "0";
            if (nextIndex != -1) {
                nextText = ".mapX" + nextIndex;
            }
            String prevText = "0";
            if (prevIndex != -1) {
                prevText = ".mapX" + prevIndex;
            }
            System.out.println("\t+AnimationEnemyMapPlacement_PosBlock " + rangesX[i].size() + " , " + nextText + " , " + prevText);
            for (int key : rangesX[i].keySet()) {
                int activationObjectIndex = rangesX[i].get(key);
                System.out.println("\t+AnimationEnemyMapPlacement_PosBlockEntry .placedEnemy" + activationObjectIndex + "\t\t; " + activationObjects.get(activationObjectIndex).getYpos());
            }
            prevIndex = i;
        }

        // Generate Y list
        System.out.println();
        System.out.println("AnimationEnemyMapPlacementsY");
        prevIndex = -1;
        for (int i = 0 ; i < rangesY.length ; i++) {
            if (rangesY[i] == null) {
                continue;
            }
            int nextIndex = -1;
            System.out.println(".mapY" + i);
            for (int j = i+1 ; j < rangesY.length ; j++) {
                if (rangesY[j] != null) {
                    nextIndex = j;
                    break;
                }
            }
            String nextText = "0";
            if (nextIndex != -1) {
                nextText = ".mapY" + nextIndex;
            }
            String prevText = "0";
            if (prevIndex != -1) {
                prevText = ".mapY" + prevIndex;
            }
            System.out.println("\t+AnimationEnemyMapPlacement_PosBlock " + rangesY[i].size() + " , " + nextText + " , " + prevText);
            for (int key : rangesY[i].keySet()) {
                int activationObjectIndex = rangesY[i].get(key);
                System.out.println("\t+AnimationEnemyMapPlacement_PosBlockEntry .placedEnemy" + activationObjectIndex + "\t\t; " + activationObjects.get(activationObjectIndex).getXpos());
            }
            prevIndex = i;
        }


    }
}
