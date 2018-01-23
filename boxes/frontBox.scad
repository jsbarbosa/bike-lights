thickness = 1.2;

module bikeBar()
{
    rotate([0, 90, 0])
    cylinder(h = 100, d = 23, $fn = 100, center = true);
}

module screw(pos = [0,0,0], d = 3)
{
    translate(pos)
    //rotate([90, 0, 0])
    cylinder(h = 8, d = d, $fn=20, center = true);
}


module screwHole(pos)
{
    size = 3;
    translate(pos)
    difference()
    {
        cube([10, 10, size], center = true);
        
        translate([0, 0, 1.1])
        screw();
    }
}

module frontBox()
{
    difference()
    {
        union()
        {
            translate([0, 0, -14])
            difference()
            {
                translate([0, 0, 23/3])
                cube([50, 23 + 2*thickness, 23/2 + thickness], center = true);
                bikeBar();
                cube([30, 30, 20], center = true);
            }
            
            translate([0, 0, 45/2])
            difference()
            {
                cube([50, 35, 45], center = true);
                translate([0, 0, thickness])
                cube([50 - 2*thickness, 35 - 2*thickness, 45], center = true);
            }
        }
            
        translate([15, 10, 20])
        cube([14, 20, 21], center = true);
        
        translate([-8, 10, 30])
        rotate([90, 0, 0])
        cylinder(h = 32, d = 12, center = true);
        
        translate([-8, 15, 13])
        cube([24, 14, 13], center = true);
        
        translate([12, -15, 12])
        rotate([90, 0, 0])
        screw(d = 7);
        
        translate([0, -15, 10])
        cube([8, 10, 2], center = true);
        
        translate([20, 15, -8])
        rotate([45, 0, 0])
        cube([5, 3, 50], center = true);
        
        translate([20, -15, -8])
        rotate([-45, 0, 0])
        cube([5, 3, 50], center = true);
        
        translate([-20, 15, -8])
        rotate([45, 0, 0])
        cube([5, 3, 50], center = true);
        
        translate([-20, -15, -8])
        rotate([-45, 0, 0])
        cube([5, 3, 50], center = true);
    }
    
    screwHole([50/2 - 5, 34/2 - 5, 42]);
    screwHole([-50/2 + 5, 34/2 - 5, 42]);
    screwHole([50/2 - 5, -34/2 + 5, 42]);
    screwHole([-50/2 + 5, -34/2 + 5, 42]);
}

module frontBox2(pos = [0, 0, 0])
{
    translate(pos)
    difference()
    {
        cube([50 - 2*thickness, 35 - 2*thickness, thickness], center = true);
        screw([50/2 - 5, 34/2 - 5, 0]);
        screw([-50/2 + 5, 34/2 - 5, 0]);
        screw([50/2 - 5, -34/2 + 5, 0]);
        screw([-50/2 + 5, -34/2 + 5, 0]);
    }
}


frontBox();

//frontBox2([0, 0, 44]);