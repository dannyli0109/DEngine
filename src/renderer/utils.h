#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm> // std::sort
#include "graphicsInclude.h"

namespace DEngine
{
    std::vector<glm::vec2> generateConvexHullPoints(const std::string &imagePath)
    {
        int width, height, channels;
        unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);

        if (!data)
        {
            std::cerr << "Failed to load image: " << imagePath << std::endl;
            return {};
        }

        std::vector<glm::vec2> points;
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                int index = (y * width + x) * channels;
                // Check if the pixel is not transparent
                if (channels == 4 && data[index + 3] != 0)
                {
                    points.push_back(glm::vec2(x, y));
                }
            }
        }

        stbi_image_free(data);
        return points;
    }
    glm::vec2 p0;

    // A utility function to find next to top in a stack
    glm::vec2 nextToTop(std::stack<glm::vec2> &S)
    {
        glm::vec2 p = S.top();
        S.pop();
        glm::vec2 res = S.top();
        S.push(p);
        return res;
    }

    // A utility function to return square of distance between p1 and p2
    int distSq(glm::vec2 p1, glm::vec2 p2)
    {
        return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
    }

    // To find orientation of ordered triplet (p, q, r).
    // The function returns following values
    // 0 --> p, q and r are colinear
    // 1 --> Clockwise
    // 2 --> Counterclockwise
    int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r)
    {
        int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

        if (val == 0)
            return 0;             // colinear
        return (val > 0) ? 1 : 2; // clock or counterclock wise
    }

    // A function used by std::sort() to sort points with respect to the first point
    // Used in convexHull()
    bool compare(const glm::vec2 &p1, const glm::vec2 &p2)
    {
        int o = orientation(p0, p1, p2);
        if (o == 0)
            return (distSq(p0, p2) >= distSq(p0, p1));
        return (o == 2);
    }

    // Prints convex hull of a set of n points.
    std::vector<glm::vec2> convexHull(std::vector<glm::vec2> points, int n)
    {
        // Find the bottommost point
        int ymin = points[0].y, min = 0;
        for (int i = 1; i < n; i++)
        {
            int y = points[i].y;

            // Pick the bottom-most or chose the left
            // most point in case of tie
            if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
                ymin = points[i].y, min = i;
        }

        // Place the bottom-most point at first position
        std::swap(points[0], points[min]);

        // Sort n-1 points with respect to the first point.
        // A point p1 comes before p2 in sorted output if p2
        // has larger polar angle (in counterclockwise
        // direction) than p1
        p0 = points[0];
        std::sort(points.begin(), points.end(), compare);

        // If two or more points make same angle with p0,
        // Remove all but the one that is farthest from p0
        // Remember that, in above sorting, our criteria was
        // to keep the farthest point at the end when more than
        // one points have same angle.
        int m = 1; // Initialize size of modified array
        for (int i = 1; i < n; i++)
        {
            // Keep removing i while angle of i and i+1 is same
            // with respect to p0
            while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
                i++;

            points[m] = points[i];
            m++; // Update size of modified array
        }

        // If modified array of points has less than 3 points,
        // convex hull is not possible
        if (m < 3)
            return {};

        // Create an empty stack and push first three points
        // to it.
        std::stack<glm::vec2> S;
        S.push(points[0]);
        S.push(points[1]);
        S.push(points[2]);

        // Process remaining n-3 points
        for (int i = 3; i < m; i++)
        {
            // Keep removing top while the angle formed by
            // points next-to-top, top, and points[i] makes
            // a non-left turn
            while (S.size() > 1 && orientation(nextToTop(S), S.top(), points[i]) != 2)
                S.pop();
            S.push(points[i]);
        }

        // Now stack has the output points, print contents of stack
        std::vector<glm::vec2> hull;
        while (!S.empty())
        {
            glm::vec2 p = S.top();
            hull.push_back(p);
            S.pop();
        }
        return hull;
    }

}