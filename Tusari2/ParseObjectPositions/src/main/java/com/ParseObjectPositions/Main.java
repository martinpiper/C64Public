package com.ParseObjectPositions;

import org.w3c.dom.*;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.*;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;

public class Main {

    private static void checkForFileBasedData(String directory, String currentName, Properties properties, String type) throws IOException {
        try (BufferedReader br = new BufferedReader(new FileReader(Paths.get(directory , currentName + "." + type + ".txt").toFile()))) {
            String line;
            int index = 0;
            while ((line = br.readLine()) != null) {
                if (!line.trim().isEmpty()) {
                    String propName = currentName + "." + type + "." + index;
                    if (!properties.containsKey(propName)) {
                        // TODO: Could insert a comment to indicate that this comes from the file?
                        properties.setProperty(propName, line);
                    }
                    index++;
                }
            }
        } catch (FileNotFoundException ignored) {}
    }

    static public void main(String[] args) throws IOException, ParserConfigurationException, SAXException {
        PrintWriter outPlacement = new PrintWriter(new File(args[2]+"Placement.a"));
        generatedWarning(outPlacement);
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
                for (int i = 3 ; i < args.length ; i++) {
                    checkForFileBasedData(args[i], currentName, properties, "data");
                    checkForFileBasedData(args[i], currentName, properties, "activation");
                    checkForFileBasedData(args[i], currentName, properties, "code");
                }
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

        LinkedHashSet<String> fileData = new LinkedHashSet<String>();
        LinkedHashSet<String> fileActivation = new LinkedHashSet<String>();
        LinkedHashSet<String> fileCode = new LinkedHashSet<String>();

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

                for (int a = 0 ; a < 100 ; a++) {
                    String value = properties.getProperty(objectName+".data."+Integer.toString(a));
                    if (value != null && !value.isEmpty()) {
                        if (value.charAt(0) == '_') {
                            value = '\t' + value.substring(1);
                        }
                        fileData.add(value);
                    }
                    value = properties.getProperty(objectName+".activation."+Integer.toString(a));
                    if (value != null && !value.isEmpty()) {
                        if (value.charAt(0) == '_') {
                            value = '\t' + value.substring(1);
                        }
                        fileActivation.add(value);
                    }
                    value = properties.getProperty(objectName+".code."+Integer.toString(a));
                    if (value != null && !value.isEmpty()) {
                        if (value.charAt(0) == '_') {
                            value = '\t' + value.substring(1);
                        }
                        fileCode.add(value);
                    }
                }


                ActivationObject activationObject = new ActivationObject(objectName,x ,y-21);
                if (activationObject.getXpos() > maxX) {
                    maxX = activationObject.getXpos();
                }
                if (activationObject.getYpos() > maxY) {
                    maxY = activationObject.getYpos();
                }
                outPlacement.println("\t\t+AnimationEnemyMapPlacement ~.placedEnemy" + activationObjects.size()+" , " + objectName + "\t, " + activationObject.getXpos() +" , " + activationObject.getYpos());
                activationObjects.add(activationObject);
            }
        }

        outPlacement.println();
        outPlacement.println("; Num: + " + activationObjects.size() + " Max: " + maxX + " , " + maxY);

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
        outPlacement.println("AnimationEnemyMapPlacementsX");
        int prevIndex = -1;
        for (int i = 0 ; i < rangesX.length ; i++) {
            if (rangesX[i] == null) {
                continue;
            }
            int nextIndex = -1;
            outPlacement.println(".mapX" + i);
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
            outPlacement.println("\t+AnimationEnemyMapPlacement_PosBlock " + rangesX[i].size() + " , " + nextText + " , " + prevText);
            for (int key : rangesX[i].keySet()) {
                int activationObjectIndex = rangesX[i].get(key);
                outPlacement.println("\t+AnimationEnemyMapPlacement_PosBlockEntry .placedEnemy" + activationObjectIndex + "\t\t; " + activationObjects.get(activationObjectIndex).getYpos());
            }
            prevIndex = i;
        }

        // Generate Y list
        outPlacement.println();
        outPlacement.println("AnimationEnemyMapPlacementsY");
        prevIndex = -1;
        for (int i = 0 ; i < rangesY.length ; i++) {
            if (rangesY[i] == null) {
                continue;
            }
            int nextIndex = -1;
            outPlacement.println(".mapY" + i);
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
            outPlacement.println("\t+AnimationEnemyMapPlacement_PosBlock " + rangesY[i].size() + " , " + nextText + " , " + prevText);
            for (int key : rangesY[i].keySet()) {
                int activationObjectIndex = rangesY[i].get(key);
                outPlacement.println("\t+AnimationEnemyMapPlacement_PosBlockEntry .placedEnemy" + activationObjectIndex + "\t\t; " + activationObjects.get(activationObjectIndex).getXpos());
            }
            prevIndex = i;
        }

        outPlacement.flush();
        outPlacement.close();

        PrintWriter outData = new PrintWriter(new File(args[2]+"Data.a"));
        generatedWarning(outData);
        PrintWriter outActivation = new PrintWriter(new File(args[2]+"Activation.a"));
        generatedWarning(outActivation);
        PrintWriter outCode = new PrintWriter(new File(args[2]+"Code.a"));
        generatedWarning(outCode);

        for (String line : fileData) {
            outData.println(line);
        }
        for (String line : fileActivation) {
            outActivation.println(line);
        }
        for (String line : fileCode) {
            outCode.println(line);
        }

        outData.flush();
        outData.close();
        outActivation.flush();
        outActivation.close();
        outCode.flush();
        outCode.close();
    }



    private static void generatedWarning(PrintWriter outPlacement) {
        outPlacement.println("; This file is automatically generated, do not edit!");
    }
}
