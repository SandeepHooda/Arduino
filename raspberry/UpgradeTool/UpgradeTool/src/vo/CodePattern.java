package vo;

import java.util.regex.Pattern;

public class CodePattern {
	private String findPattern;
	private String replaceWith;
	private Pattern compiledPattern;
	public String getFindPattern() {
		return findPattern;
	}
	public void setFindPattern(String findPattern) {
		this.findPattern = findPattern;
	}
	
	public Pattern getCompiledPattern() {
		return compiledPattern;
	}
	public void setCompiledPattern(Pattern compiledPattern) {
		this.compiledPattern = compiledPattern;
	}
	public String getReplaceWith() {
		return replaceWith;
	}
	public void setReplaceWith(String replaceWith) {
		this.replaceWith = replaceWith;
	}

}
