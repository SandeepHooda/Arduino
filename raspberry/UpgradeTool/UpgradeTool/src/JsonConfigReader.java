import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import vo.CodePattern;

public class JsonConfigReader {

	
	
	public static List<CodePattern> readPatternConfig(String filePath){
		List<CodePattern> patternList = new ArrayList<CodePattern>();
		
		 JSONParser jsonParser = new JSONParser();
		 try (FileReader reader = new FileReader(filePath))
	        {
	            //Read JSON file
	            Object obj = jsonParser.parse(reader);
	 
	            JSONArray patternListConfig = (JSONArray) obj;
	            
	            //Iterate
	             
	            for (int i =0;i<patternListConfig.size();i++) {
	            	patternList.add(compilePattern( (JSONObject) patternListConfig.get(i) ));
	            }
	            
	 
	        } catch (FileNotFoundException e) {
	            e.printStackTrace();
	        } catch (IOException e) {
	            e.printStackTrace();
	        } catch (ParseException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		return patternList;
	}
	
	private static CodePattern compilePattern(JSONObject aConfigEntry) 
    {
		CodePattern pattern = new CodePattern();
		JSONObject configObject = (JSONObject) aConfigEntry.get("replacement");
		pattern.setFindPattern((String) configObject.get("findPattern"));
		pattern.setReplaceWith((String) configObject.get("replaceWith"));
		Pattern compiledPattern = Pattern.compile(pattern.getFindPattern(),Pattern.DOTALL);
		pattern.setCompiledPattern(compiledPattern);
        return pattern;
      
    }


}
