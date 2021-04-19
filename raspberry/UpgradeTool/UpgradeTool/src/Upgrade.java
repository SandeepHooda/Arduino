import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.List;
import java.util.Scanner;
import java.util.regex.Matcher;

import vo.CodePattern;

public class Upgrade {

	private static List<CodePattern> patternList;
	private static String rootDir = "";
	private static String rootDir_unixPath = "";
	private static boolean addcomments = false;
	public static void main(String[] args) {
		System.out.println("Start");
		
		if (null == args || args.length == 0) {
			System.out.println("Please provide name of the root directory where you have place the code and config.json");
			return ;
		}
		rootDir = args[0];
		if (args.length >=2 && "true".equalsIgnoreCase(args[1])) {
			addcomments = true;
		}
		rootDir_unixPath = rootDir.replace("\\", "/");
		
		patternList= JsonConfigReader.readPatternConfig(rootDir+"\\config.json");
		upgradeFiles(rootDir+"\\src");
		
		System.out.println("End");

	}
	private static void upgradeFiles(String dir) {
		File dirObj = new File(dir);
		File[] files = dirObj.listFiles();

		 if (files != null && files.length > 0) {
	            for (File file : files) {
	                // Check if the file is a directory
	                if (file.isDirectory()) {
	                    // We will not print the directory name, just use it as a new
	                    // starting point to list files from
	                	upgradeFiles(file.getAbsolutePath());
	                } else {
	                    // We can use .length() to get the file size
	                    System.out.println("File name : "+dir+"\\"+file.getName() + " (size in bytes: " + file.length()+")");
	                    //Read orignal file
	                    String fileContent = readfileContent(dir+"\\"+file.getName());
	                    
	                    //Modile the file
	                    for (CodePattern codePattern: patternList) {
	            			Matcher extendsPatternMatcher = codePattern.getCompiledPattern().matcher(fileContent);
	            			 if (extendsPatternMatcher.find()) {
	            				String match = extendsPatternMatcher.group(0);
	            				String comments = "";
	            				if (addcomments) {
	            					if (file.getName().endsWith("java")) {
	            						comments = "/* Modified by upgrade tool /*";
	            					}else if (file.getName().endsWith("html")) {
	            						comments = "<!-- Modified by upgrade tool -->";
	            					}else if (file.getName().endsWith("jsp")) {
	            						comments = "<%-- Modified by upgrade tool --%>";
	            					}
	            				}
	            				fileContent = fileContent.replace(match, codePattern.getReplaceWith()+comments);
	            			 }
	            		}
	                    
	                    //Write the modifiled file
	                    
	                    upgradeFile( dir,  file,  fileContent );
	                  
	                }
	            }
	        }
	}
	
	private static void upgradeFile(String dir, File file, String fileContent ) {
		 FileWriter myWriter;
			try {
				String orignalPath = dir+"\\"+file.getName();
				orignalPath = orignalPath.replace("\\", "/");
				String filepath = (orignalPath.replaceAll(rootDir_unixPath, ""));
				String upgradedfilePath = rootDir_unixPath+"/upgraded"+filepath;
				
				File upgradedfile = new File(upgradedfilePath);
				upgradedfile.getParentFile().mkdirs();
				upgradedfile.createNewFile();
				myWriter = new FileWriter(upgradedfilePath);
				myWriter.write(fileContent);
             myWriter.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	}
	private static String readfileContent(String filepath) {
		  try {
		      File myObj = new File(filepath);
		      Scanner myReader = new Scanner(myObj);
		      StringBuilder sb = new StringBuilder();
		      while (myReader.hasNextLine()) {
		    	  sb.append( myReader.nextLine()+"\r\n");
		       
		      }
		      myReader.close();
		      return sb.toString();
		    } catch (FileNotFoundException e) {
		      System.out.println("An error occurred.");
		      e.printStackTrace();
		    }
		  return "";
	}

}
