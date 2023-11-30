class Cube {

private:
    unsigned int textureId;
    int height;
    int width;
    int depth;

    float centerX;
    float centerY;
    float centerZ;

public:
    Cube(int width = 10, int height = 10, int depth = 10, const char *textureFile = "");

    void loadTexture(const char *filename);
    void DrawCube(float centerX, float centerY, float centerZ);
};