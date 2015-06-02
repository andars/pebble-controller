package com.andars.pcserver;

import static spark.Spark.*;
import java.util.concurrent.TimeUnit;
import java.io.File;

public class App {
    public static void main( String[] args ) {

        port(5000);

        post("/data", (request, response) -> {
            String button = request.queryParams("button");

            String home = System.getProperty("user.home");

            String handlerPath = home + File.separator + ".pc_handlers" + File.separator + button;
            System.out.println(handlerPath);

            Process handler = new ProcessBuilder(handlerPath).start();
            handler.waitFor(5, TimeUnit.SECONDS);
            handler.destroy();

            response.status(200);
            return "thanks";
        });
    }
}
