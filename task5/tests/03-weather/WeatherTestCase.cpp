//
// Created by Pavel Akhtyamov on 02.05.2020.
//

#include "WeatherTestCase.h"
#include "WeatherMock.h"

#include <sstream>


TEST_F(WeatherTestCase, BadResponse) {
  WeatherMock w;
  cpr::Response response;
  response.status_code = 401;
  response.text = "api key is invalid";

  EXPECT_CALL(w, Get("Mosco")).WillOnce(testing::Return(response));

  ASSERT_THROW(w.GetTemperature("Mosco"), std::invalid_argument);
}

TEST_F(WeatherTestCase, StringDifference) {
  WeatherMock w;

  json r1, r2;
  r1["list"][0]["main"]["temp"] = 10.0;
  r2["list"][0]["main"]["temp"] = 20.0;

  std::string c1 = "Moscow", c2 = "Tokio";

  std::ostringstream gr, lo;

  gr  << "Weather in " << c2 << " is warmer than in " << c1 << " by 10 degrees";
  lo  << "Weather in " << c1 << " is colder than in " << c2 << " by 10 degrees";

  cpr::Response resp1, resp2;
  resp1.status_code = 200;
  resp2.status_code = 200;
  resp1.text = r1.dump();
  resp2.text = r2.dump();

  EXPECT_CALL(w, Get(c1)).WillRepeatedly(testing::Return(resp1));
  EXPECT_CALL(w, Get(c2)).WillRepeatedly(testing::Return(resp2));

  ASSERT_EQ(w.GetDifferenceString(c1, c2), lo.str());
  ASSERT_EQ(w.GetDifferenceString(c2, c1), gr.str());
}

TEST_F(WeatherTestCase, OK) {
    WeatherMock w;
    const std::string api_key = "238923sdf";
    const std::string cities[] = {"Moscow", "Barcelona", "Milan", "Madrid", "Paris", "Lissabon"};
    json simple_json;
    simple_json["list"][0]["main"]["temp"] = -22.0;

    cpr::Response simple_response;
    simple_response.status_code = 200;
    simple_response.text = simple_json.dump();

    for (const auto& city : cities) {
      EXPECT_CALL(w, Get(city))
        .WillOnce(testing::Return(simple_response));
    }

    EXPECT_CALL(w, SetApiKey(api_key)).Times(1);

    ASSERT_EQ(w.GetResponseForCity(cities[0]), simple_json);
    ASSERT_EQ(w.GetTemperature(cities[1]), -22.0);
    ASSERT_EQ(w.FindDiffBetweenTwoCities(cities[2], cities[3]), 0);
    
    w.SetApiKey(api_key);

    ASSERT_TRUE(w.GetDifferenceString(cities[4], cities[5]).find('0') != std::string::npos);
}

TEST_F(WeatherTestCase, ApiKeyIsSet) {
  Weather w;
  ASSERT_NO_THROW(w.SetApiKey("JSDL WEEHsjdsklfkljjdskf"));
}
