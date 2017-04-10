package com.iot;

import java.io.IOException;
import java.net.URL;
import java.util.Date;
import java.util.TimeZone;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.appengine.api.urlfetch.FetchOptions;
import com.google.appengine.api.urlfetch.HTTPHeader;
import com.google.appengine.api.urlfetch.HTTPMethod;
import com.google.appengine.api.urlfetch.HTTPRequest;
import com.google.appengine.api.urlfetch.HTTPResponse;
import com.google.appengine.api.urlfetch.URLFetchService;
import com.google.appengine.api.urlfetch.URLFetchServiceFactory;

/**
 * Servlet implementation class PutData
 */
public class PutData extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public PutData() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		URLFetchService fetcher = URLFetchServiceFactory.getURLFetchService();
		FetchOptions lFetchOptions = FetchOptions.Builder.doNotValidateCertificate();
		String collection = request.getParameter("collection");
		String datakey = request.getParameter("datakey");
		TimeZone.setDefault(TimeZone.getTimeZone("IST"));
		String value = request.getParameter("value") + " @ "+ (new Date());
		
		String httpsURL = "https://api.mlab.com/api/1/databases/sandeepdb/collections/"+collection+"?apiKey=soblgT7uxiAE6RsBOGwI9ZuLmcCgcvh_";
		String respo = "";
		 try {
			
		        URL url = new URL(httpsURL);
	            HTTPRequest req = new HTTPRequest(url, HTTPMethod.PUT, lFetchOptions);
	            HTTPHeader header = new HTTPHeader("Content-type", "application/json");
	            req.setHeader(header);
	            req.setPayload(("{\""+datakey+"\":\""+value+"\"}").getBytes());
	            HTTPResponse res = fetcher.fetch(req);
	            respo =(new String(res.getContent()));
	 
	        } catch (IOException e) {
	        	respo = e.getMessage();
	        }
		
	  
		 response.setContentType("text/plain");
		
		
		 response.getWriter().println(respo);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

}
