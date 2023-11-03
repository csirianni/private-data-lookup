// #include <catch2/catch_test_macros.hpp>

// /*
//     Testing the server can run and passwords will be posted.
// */
// void test_api(crow::App<crow::CORSHandler> *curr_app)
// {
//     curr_app->validate(); // Used to make sure all the route handlers are in order.

//     {
//         crow::request req;
//         crow::response res;

//         // Test the endpoint for running the server
//         req.url = "/";
//         curr_app->handle(req, res);
//         if (res.code == 200)
//         {
//             boost_swap_impl::cout << "Server test: success connecting to endpoint to run the server \n\n";
//         }
//         else
//         {
//             boost_swap_impl::cerr << "Set Intersection Test error: " + boost_swap_impl::to_string(res.code) + "\n\n";
//         }

//         // Test the endpoint for compute set intersection
//         req.url = "/passwords";
//         req.method = "POST"_method;
//         req.add_header("Access-Control-Allow-Headers", "*");
//         req.add_header("Content-Type", "application/json");
//         req.body = "TestPass1&";

//         // res will contain a code of 200, and a response body of "success"
//         curr_app->handle(req, res);
//         if (res.code == 200)
//         {
//             boost_swap_impl::cout << "Set Intersection Test: success connecting to endpoint to compute set intersection \n\n";
//         }
//         else
//         {
//             boost_swap_impl::cerr << "Set Intersection Test error: " + boost_swap_impl::to_string(res.code) + "\n\n";
//         }
//     }
// }
